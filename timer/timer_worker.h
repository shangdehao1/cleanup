#ifndef TIMER_WORKER_H_
#define TIMER_WORKER_H_

#include <functional>

#include "common/smart_ptr/networking_ptr.h"
#include "common/lock/locks.h"
#include "common/common_internal.h"
#include "timer/ptime.h"

namespace hdcs {
namespace networking {

class TimerWorker : public hdcs::networking::enable_shared_from_this<TimerWorker>
{
public:
    // boost::posix_time::ptime
    typedef std::function<void(const PTime& )> WorkRoutine;

public:
    TimerWorker(IOService& io_service)
        : _io_service(io_service)
        , _is_running(false)
        , _time_duration(time_duration_seconds(1))
        , _work_routine(NULL)
        , _timer(io_service)
        , _strand(io_service)
    {}

    ~TimerWorker()
    {
        stop();
    }

    bool is_running()
    {
        return _is_running;
    }

    void set_time_duration(const TimeDuration& time_duration)
    {
        _time_duration = time_duration;
    }

    void set_work_routine(const WorkRoutine& work_routine)
    {
        _work_routine = work_routine;
    }

    void start()
    {
        if (_is_running) return;

        _is_running = true;

        ScopedLocker<MutexLock> _(_timer_lock);
        // from now begin, after _time_duration end.
        _timer.expires_from_now(_time_duration);
        _timer.async_wait(
                _strand.wrap(
                    boost::bind(&TimerWorker::on_timeout, shared_from_this(), _1)
                    ));
    }

    void stop()
    {
        if (!_is_running) return;
        _is_running = false;

        ScopedLocker<MutexLock> _(_timer_lock);
        // timer will be canceled.
        // all callback of async_operation will be called, and error will be equal to operation_aborted.
        _timer.cancel();
    }

private:
    // when async_operation be canceled, or
    // timeout
    void on_timeout(const boost::system::error_code& ec)
    {
        if (_is_running)
        {
            PTime now = ptime_now();

            if (ec != boost::asio::error::operation_aborted && _work_routine)
            {
                _work_routine(now);
            }

            ScopedLocker<MutexLock> _(_timer_lock);
            _timer.expires_at(now + _time_duration);
            _timer.async_wait(_strand.wrap(boost::bind(
                            &TimerWorker::on_timeout, shared_from_this(), _1)));
        }
    }

private:
    IOService& _io_service;
    volatile bool _is_running;

    // boost::posix_time::time_duration
    TimeDuration _time_duration;
    // function object.
    WorkRoutine _work_routine;
    // boost::asio::deadline_timer
    IOServiceTimer _timer;
    MutexLock _timer_lock;
    IOServiceStrand _strand;

}; // timer_worker

} // networking 
} // hdcs
#endif
