// Copyright (c) 2014 Baidu.com, Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THREAD_GROUP_H_
#define THREAD_GROUP_H_

#include "../common/common.h"
#include "../closure/ext_closure.h"

namespace hdcs {
namespace networking {

// Defined in this file.
class ThreadGroup;
typedef hdcs::networking::shared_ptr<ThreadGroup> ThreadGroupPtr;

// Defined in other files.
class ThreadGroupImpl;

class ThreadGroup
{
public:
    // Constructor.  User should specify the "thread_num", which can not be changed
    // afterwards.  All the threads will start running after contruct done.
    ThreadGroup(int thread_num);

    // Destructor.  It will join all threads, so user must ensure that all threads
    // will exit eventually.
    ~ThreadGroup();

    // Get the number of threads in this thread group.
    int thread_num() const;

    // Request the thread group to invoke the given handler.
    // The handler may be executed inside this function if the guarantee can be met.
    // The "handler" should be a self delete closure, which can be created through
    // NewClosure().
    void dispatch(google::protobuf::Closure* handler);

    // Request the thread group to invoke the given handler and return immediately.
    // It guarantees that the handler will not be called from inside this function.
    // The "handler" should be a self delete closure, which can be created through
    // NewClosure().
    void post(google::protobuf::Closure* handler);

    // Request the thread group to invoke the given handler.
    // The handler may be executed inside this function if the guarantee can be met.
    // The "handler" should be a self delete closure, which can be created through
    // NewExtClosure().
    void dispatch(ExtClosure<void()>* handler);

    // Request the thread group to invoke the given handler and return immediately.
    // It guarantees that the handler will not be called from inside this function.
    // The "handler" should be a self delete closure, which can be created through
    // NewExtClosure().
    void post(ExtClosure<void()>* handler);

private:
    hdcs::networking::shared_ptr<ThreadGroupImpl> _imp;

    //SOFA_PBRPC_DISALLOW_EVIL_CONSTRUCTORS(ThreadGroup);
};

} // namespace
} // namespace

#endif // 
