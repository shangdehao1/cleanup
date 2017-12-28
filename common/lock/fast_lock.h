// Copyright (c) 2014 Baidu.com, Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FAST_LOCK_H_
#define FAST_LOCK_H_

#include "mutex_lock.h"
#include "spin_lock.h"

namespace hdcs {
namespace networking {

#if defined( SOFA_PBRPC_USE_SPINLOCK )
    typedef SpinLock FastLock;
#else
    typedef MutexLock FastLock;
#endif // defined( SOFA_PBRPC_USE_SPINLOCK )

}
}

#endif
