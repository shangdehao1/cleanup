#ifndef TIMEOUT_MANAGER_IMPL_H_
#define TIMEOUT_MANAGER_IMPL_H_

#include <vector>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include "common/common_internal.h"
#include "closure/closure.h"
#include "closure/ext_closure.h"
#include "common/lock/locks.h"
#include "timer/timeout_manager.h"
#include "timer/timer_worker.h"
#include "io_service/thread_group_impl.h"

namespace hdcs {
namespace networking {

class TimeoutManagerImpl;
typedef hdcs::networking::shared_ptr<TimeoutManagerImpl> TimeoutManagerImplPtr;

class TimeoutManagerImpl : public hdcs::networking::enable_shared_from_this<TimeoutManagerImpl>
{
public:
    // Thread number for timer and callbacks.
    const static int kThreadCount = 1;
    // Timeout granularity of timer in milli-seconds.
    const static int64_t kTimerGranularity = 10; 

    typedef TimeoutManager::Id Id;
    typedef TimeoutManager::Type Type;
    typedef TimeoutManager::Callback Callback;

    TimeoutManagerImpl();
    ~TimeoutManagerImpl();

    bool is_running();

    void start();

    void stop();

    void clear();

    Id add(int64_t interval, Callback* callback);

    Id add_repeating(int64_t interval, Callback* callback);

    bool erase(Id id);

private:
    // Given interval in milli-seconds, calculate expiration ticks.
    inline int64_t calc_expiration(int64_t interval)
    {
        return _last_ticks + time_duration_milliseconds(interval).ticks() + _rectify_ticks;
    }

    void timer_run(const PTime& now);

private:
    struct Event {
        Id id;
        int64_t expiration;
        int64_t repeat_interval;
        Callback* callback;
        Event(Id i, int64_t e, int64_t r, Callback* c)
            : id(i), expiration(e), repeat_interval(r), callback(c) {}
    };

    typedef boost::multi_index_container<
        Event,
        boost::multi_index::indexed_by<
            boost::multi_index::ordered_unique<boost::multi_index::member<
            Event, Id, &Event::id
            > >,
        boost::multi_index::ordered_non_unique<boost::multi_index::member<
            Event, int64_t, &Event::expiration
            > >
        >
    > Set;

    enum {
        BY_ID=0,
        BY_EXPIRATION=1
    };

    typedef Set::nth_index<BY_ID>::type IdIndex;
    typedef Set::nth_index<BY_EXPIRATION>::type ExpirationIndex;
    typedef std::vector<Event> EventVec;

    volatile bool _is_running;
    MutexLock _start_stop_lock;
    PTime _epoch_time;
    volatile int64_t _last_ticks; 
    int64_t _rectify_ticks;

    ThreadGroupImplPtr _thread_group;
    TimerWorkerPtr _timer_worker;

    Set _timeouts;
    Id _next_id;
    MutexLock _timeouts_lock;
};

} // namespace hdcs
} // namespace networking

#endif
