// Copyright (c) 2014 Baidu.com, Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SPIN_LOCK_H_
#define SPIN_LOCK_H_

// XOS
#ifdef __APPLE__
#include <libkern/OSAtomic.h>

namespace sofa {
namespace pbrpc {

class SpinLock
{
public:
    SpinLock(): _lock(0) { }
    ~SpinLock() { }
    void lock() { OSSpinLockLock(&_lock); }
    bool try_lock() { return OSSpinLockTry(&_lock); }
    void unlock() { OSSpinLockUnlock(&_lock); }

private:
    OSSpinLock _lock;
}; // class SpinLock

}
}

#else

#include <pthread.h>

namespace sofa {
namespace pbrpc {

class SpinLock
{
public:
    SpinLock() { pthread_spin_init(&_lock, 0); }
    ~SpinLock() { pthread_spin_destroy(&_lock); }
    void lock() { pthread_spin_lock(&_lock); }
    bool try_lock() { return pthread_spin_trylock(&_lock) == 0; }
    void unlock() { pthread_spin_unlock(&_lock); }

private:
    pthread_spinlock_t _lock;
}; // class SpinLock

}
} 
#endif

#endif
