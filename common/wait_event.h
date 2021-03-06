#ifndef WAIT_EVENT_H_
#define WAIT_EVENT_H_

#include <pthread.h>

namespace hdcs {
namespace networking {

class WaitEvent
{
public:
    WaitEvent() : _signaled(false)
    {
        pthread_mutex_init(&_lock, NULL);
        pthread_cond_init(&_cond, NULL);
    }
    ~WaitEvent() 
    {
        pthread_mutex_destroy(&_lock);
        pthread_cond_destroy(&_cond);
    }

    void Wait()
    {
        pthread_mutex_lock(&_lock);
        while (!_signaled)
        {
            pthread_cond_wait(&_cond, &_lock);
        }
        _signaled = false;
        pthread_mutex_unlock(&_lock);
    }

    void Signal()
    {
        pthread_mutex_lock(&_lock);
        _signaled = true;
        pthread_cond_signal(&_cond);
        pthread_mutex_unlock(&_lock);
    }

private:
    pthread_mutex_t _lock;
    pthread_cond_t _cond;
    bool _signaled;
};

} // namespace 
} // namespace 

#endif // 
