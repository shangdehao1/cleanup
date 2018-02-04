#ifndef H_TIMEOUT_MANAGER_H_
#define H_TIMEOUT_MANAGER_H_

#include <queue>
#include <set>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include "controller/controller.h"
#include "controller/controller_impl.h"
#include "stream/client/client_impl.h"

namespace hdcs {
namespace networking {

class NTimeoutManager : public hdcs::networking::enable_shared_from_this<NTimeoutManager>
{
public:
    NTimeoutManager(IOService& io_service)
        : _io_service(io_service)
        , _is_running(false)
        , _epoch_time(ptime_now()) // time pointer.
        , _next_id(1u)
    {}

    ~NTimeoutManager()
    {
        stop();
    }

    bool is_running() const
    {
        return _is_running;
    }

    void start()
    {
        ScopedLocker<MutexLock> _(_start_stop_lock);
        if (_is_running)
            return;
        _is_running = true;

        _timer_worker.reset(new TimerWorker(_io_service));
        _timer_worker->set_time_duration(time_duration_milliseconds(100));
        _timer_worker->set_work_routine(boost::bind(
                    &NTimeoutManager::timer_run, shared_from_this(), _1));
        _timer_worker->start();
    }

    void stop()
    {
        ScopedLocker<MutexLock> _(_start_stop_lock);
        if (!_is_running)
        {
            return;
        }
        _is_running = false;

        _timer_worker->stop();

        // directly cleanup all pending timeout_event.
        clear();
        _timer_worker.reset();
    }

    // Add a timeout event.
    // If "expiration" of the "cntl" is a special value or already timeout, return false.
    // Else put the "cntl" into the manager, and return true.
    bool add(const ControllerImplPtr& cntl)
    {
        const PTime& expiration = cntl->Expiration();
        if (expiration.is_special() || expiration <= _epoch_time)
        {
            cntl->SetTimeoutId(0u);
            return false;
        }
        int64_t expiration_ticks = (expiration - _epoch_time).ticks();
        {
            ScopedLocker<FastLock> _(_add_list_lock);
            uint64_t timeout_id = _next_id++;
            cntl->SetTimeoutId(timeout_id);
            // push adding task into timer_run to execute...
            _add_list.push_back(Event(timeout_id, expiration_ticks, cntl));
        }
        return true;
    }

    // Erase a given timeout event.
    void erase(uint64_t id)
    {
        if (id == 0u) return;
        ScopedLocker<FastLock> _(_erase_set_lock);
        // push easing task into timer_run to execute.
        _erase_set.insert(id);
    }

private:
    void clear()
    {
        {
            ScopedLocker<FastLock> _(_add_list_lock);
            _add_list.clear();
        }
        {
            ScopedLocker<FastLock> _(_erase_set_lock);
            _erase_set.clear();
        }
        {
            ScopedLocker<FastLock> _(_events_lock);
            _events.clear();
        }
    }
    
    // timeout rountine
    void timer_run(const PTime& now)
    {
        if (!_is_running)
        {
            return;
        }
        int64_t now_ticks = (now - _epoch_time).ticks();

        // obtain add list
        EventList tmp_add_list;
        {
            ScopedLocker<FastLock> _(_add_list_lock);
            tmp_add_list.swap(_add_list);
        }
        // obtain erase list
        IdSet tmp_erase_set;
        {
            ScopedLocker<FastLock> _(_erase_set_lock);
            tmp_erase_set.swap(_erase_set);
        }

        // accoring to time sequence, process erasing and adding event
        ScopedLocker<FastLock> _(_events_lock);
        EventList::iterator add_it = tmp_add_list.begin();
        IdSet::iterator erase_it = tmp_erase_set.begin();
        while (add_it != tmp_add_list.end() && erase_it != tmp_erase_set.end())
        {
            if (add_it->id < *erase_it)
            {
                if (add_it->expiration <= now_ticks)
                {
                    // expired, notify
                    notify_timeout(add_it->cntl);
                }
                else
                {
                    // add into EventSet
                    _events.insert(*add_it);
                }
                ++add_it;
            }
            else if (add_it->id > *erase_it)
            {
                // erase from EventSet
                _events.get<INDEX_BY_ID>().erase(*erase_it);
                ++erase_it;
            }
            else // add_it->id == *erase_it
            {
                // ignore it
                ++add_it;
                ++erase_it;
            }
        }
        // indepentantly process adding list
        while (add_it != tmp_add_list.end())
        {
            if (add_it->expiration <= now_ticks)
            {
                // expired, notify
                notify_timeout(add_it->cntl);
            }
            else
            {
                // add into EventSet
                _events.insert(*add_it); // add into EventSet
            }
            ++add_it;
        }
        // indepentantly process erasing list.
        while (erase_it != tmp_erase_set.end())
        {
            // erase from EventSet
            _events.get<INDEX_BY_ID>().erase(*erase_it);
            ++erase_it;
        }

        // process EventSet
        ExpirationIndex& exp_index = _events.get<INDEX_BY_EXPIRATION>();
        ExpirationIndex::iterator exp_end = exp_index.upper_bound(now_ticks);
        for (ExpirationIndex::iterator exp_it = exp_index.begin();
                exp_it != exp_end; ++exp_it)
        {
            // expired, notify
            notify_timeout(exp_it->cntl);
        }
        exp_index.erase(exp_index.begin(), exp_end);
    }

private:
    void notify_timeout(const ControllerImplWPtr& weak_cntl)
    {
        ControllerImplPtr cntl = weak_cntl.lock();
        if (cntl)
        {
            // ATTENTION: here we reset timeout id to 0 to avoid unnecessarily erasing
            // from timeout manager in ClientImpl::DoneCallback(), because when
            // timeout id is 0, then NTimeoutManager::erase() will do nothing.
            cntl->SetTimeoutId(0u);
            cntl->Done(HDCS_NETWORK_ERROR_REQUEST_TIMEOUT, "timeout");

            // erase from ClientStream
            ClientStreamPtr stream = cntl->ClientStream().lock();
            if (stream)
            {
                stream->erase_request(cntl->SequenceId());
            }
        }
    }

private:
// =============================Timeout Event================================
    struct Event
    {
        uint64_t id; // 0 means invalid id
        int64_t expiration;
        ControllerImplWPtr cntl;

        Event(uint64_t i, int64_t e, const ControllerImplWPtr& c)
            : id(i), expiration(e), cntl(c) {}

        Event(const Event& o)
            : id(o.id), expiration(o.expiration), cntl(o.cntl) {}

        Event& operator=(const Event& o) {
            if (this != &o) {
                id = o.id;
                expiration = o.expiration;
                cntl = o.cntl;
            }
            return *this;
        }

        void swap(Event& o) {
            std::swap(id, o.id);
            std::swap(expiration, o.expiration);
            cntl.swap(o.cntl);
        }
    };

// ============================= multi index container ================================
    typedef boost::multi_index_container<
        Event,
        boost::multi_index::indexed_by<
            boost::multi_index::ordered_unique< boost::multi_index::member<
            Event, uint64_t, &Event::id
            > >,
        boost::multi_index::ordered_non_unique< boost::multi_index::member<
            Event, int64_t, &Event::expiration
            > >
        >
    > EventSet;

    enum {
        INDEX_BY_ID = 0,
        INDEX_BY_EXPIRATION = 1,
    };
        
    typedef EventSet::nth_index<INDEX_BY_EXPIRATION>::type ExpirationIndex;
    typedef EventSet::nth_index<INDEX_BY_ID>::type IdIndex;
    typedef std::deque<Event> EventList;
    typedef std::set<uint64_t> IdSet;

    IOService& _io_service;
    volatile bool _is_running;
    MutexLock _start_stop_lock;
    PTime _epoch_time;

    uint64_t _next_id;
    EventSet _events;
    FastLock _events_lock;

    EventList _add_list; // must be in order of id
    FastLock _add_list_lock;
    IdSet _erase_set; // in order of id
    FastLock _erase_set_lock;

    TimerWorkerPtr _timer_worker;

}; // class NTimeoutManager

} // namespace networking
} // namespace hdcs

#endif 
