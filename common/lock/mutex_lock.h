// Copyright (c) 2014 Baidu.com, Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MUTEX_LOCK_H_
#define MUTEX_LOCK_H_

#include <pthread.h>

namespace hdcs {
namespace networking {

class ConditionVariable;

class MutexLock
{
public:
    MutexLock()
    {
        pthread_mutex_init(&_lock, NULL);
    }
    ~MutexLock()
    {
        pthread_mutex_destroy(&_lock);
    }
    void lock()
    {
        pthread_mutex_lock(&_lock);
    }
    void unlock()
    {
        pthread_mutex_unlock(&_lock);
    }
private:
    friend class ConditionVariable;
    pthread_mutex_t _lock;
};

} // namespace 
} // namespace 

#endif
