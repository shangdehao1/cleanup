// Copyright (c) 2014 Baidu.com, Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SCOPED_LOCKER_H_
#define SCOPED_LOCKER_H_

namespace hdcs {
namespace networking {

template <typename LockType>
class ScopedLocker
{
public:
    explicit ScopedLocker(LockType& lock)
        : _lock(&lock)
    {
        _lock->lock();
    }

    explicit ScopedLocker(LockType* lock)
        : _lock(lock)
    {
        _lock->lock();
    }

    ~ScopedLocker()
    {
        _lock->unlock();
    }

private:
    LockType* _lock;
}; // class ScopedLocker

} // namespace
} // namespace

#endif
