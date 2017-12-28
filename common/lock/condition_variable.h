// Copyright (c) 2014 Baidu.com, Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONDITION_VARIABLE_H_
#define CONDITION_VARIABLE_H_

#include <errno.h>
#include <time.h>
#include <sys/time.h>

//#include <sofa/pbrpc/common.h>
#include "mutex_lock.h"

namespace hdcs {
namespace networking {

class ConditionVariable
{
public:
    ConditionVariable()
    {
        pthread_cond_init(&_cond, NULL);
    }
    ~ConditionVariable()
    {
        pthread_cond_destroy(&_cond);
    }
    void wait(MutexLock& mutex)
    {
        //SCHECK_EQ(0, pthread_cond_wait(&_cond, &mutex._lock));
        pthread_cond_wait(&_cond, &mutex._lock);
    }
    bool wait(MutexLock& mutex, int64_t timeout_in_ms)
    {
        if (timeout_in_ms < 0)
        {
            wait(mutex);
            return true;
        }
        timespec ts;
        calculate_expiration(timeout_in_ms, &ts);
        int error = pthread_cond_timedwait(&_cond, &mutex._lock, &ts);
        if (error == 0)
        {
            return true;
        }
        else if (error == ETIMEDOUT)
        {
            return false;
        }
        else
        {
            //SLOG(FATAL, "error no: %d", error);
            return false;
        }
    }
    void signal()
    {
        //SCHECK_EQ(0, pthread_cond_signal(&_cond));
        pthread_cond_signal(&_cond);
    }
    void broadcast()
    {
        //SCHECK_EQ(0, pthread_cond_broadcast(&_cond));
        pthread_cond_broadcast(&_cond);
    }
private:
    void calculate_expiration(int64_t timeout_in_ms, timespec* ts)
    {
        timeval tv;
        gettimeofday(&tv, NULL);
        int64_t usec = tv.tv_usec + timeout_in_ms * 1000LL;
        ts->tv_sec = tv.tv_sec + usec / 1000000;
        ts->tv_nsec = (usec % 1000000) * 1000;
    }
private:
    pthread_cond_t _cond;
};

} // 
} //

#endif
