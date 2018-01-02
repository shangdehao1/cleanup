// Copyright (c) 2014 Baidu.com, Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THREAD_GROUP_IMPL_H_
#define THREAD_GROUP_IMPL_H_

#include <unistd.h>
#include <pthread.h>
#include <cstdio>

#include <boost/bind.hpp>

#include "io_service.h"
#include "../closure/ext_closure.h"
#include "../common/common.h"

//TODO disable google::protobuf 

namespace hdcs {
namespace networking {

// Defined in this file.
class ThreadGroupImpl;
typedef hdcs::networking::shared_ptr<ThreadGroupImpl> ThreadGroupImplPtr;

// Thread init and destroy function.  Should be permanent closure.
typedef ExtClosure<bool()>* ThreadInitFunc;
typedef ExtClosure<void()>* ThreadDestFunc;

class ThreadGroupImpl
{
public:
    struct ThreadParam
    {
        int id; // sequence id in the thread group, starting from 0
        IOService* io_service;
        ThreadInitFunc init_func;
        ThreadDestFunc dest_func;
        AtomicCounter init_done;
        AtomicCounter init_fail;

        ThreadParam() : id(0), io_service(NULL), init_func(NULL), dest_func(NULL) {}
        ~ThreadParam() {}
    };
public:
    ThreadGroupImpl(int thread_num, const std::string& name = "")
        : _is_running(false)
        , _thread_num(std::max(thread_num, 1))
        , _name(name)
        , _init_func(NULL)
        , _dest_func(NULL)
        , _io_service_work(NULL)
        , _threads(NULL)
        , _thread_params(NULL)
    {
        if (_name.empty())
        {
            char tmp[20];
            sprintf(tmp, "%p", this);
            _name = tmp;
        }
    }

    ~ThreadGroupImpl()
    {
        stop();
    }

    int thread_num() const
    {
        return _thread_num;
    }

    std::string name() const
    {
        return _name;
    }

    // The "init_func" and "dest_func" should be permanent closure, and
    // owned by the caller.
    void set_init_func(ThreadInitFunc init_func)
    {
        _init_func = init_func;
    }
    void set_dest_func(ThreadDestFunc dest_func)
    {
        _dest_func = dest_func;
    }

    IOService& io_service()
    {
        return _io_service;
    }

    bool start()
    {
        if (_is_running) return true;
        _is_running = true;

        //SLOG(INFO, "start(): starting thread group [%s], thread_num=%d", _name.c_str(), _thread_num);
        _io_service_work = new IOServiceWork(_io_service);
        _threads = new pthread_t[_thread_num];
        _thread_params = new ThreadParam[_thread_num];
        for (int i = 0; i < _thread_num; ++i)
        {
            _thread_params[i].id = i;
            _thread_params[i].io_service = &_io_service;
            _thread_params[i].init_func = _init_func;
            _thread_params[i].dest_func = _dest_func;
            int ret = pthread_create(&_threads[i], NULL, &ThreadGroupImpl::thread_run, &_thread_params[i]);
            if (ret != 0)
            {
                //SLOG(ERROR, "start(): create thread[%d] failed: error=%d", i, ret);
                _thread_num = i;
                stop();
                return false;
            }
        }
        // wait for init done
        bool init_fail = false;
        while (true)
        {
            int done_num = 0;
            for (int i = 0; i < _thread_num; ++i)
            {
                if (_thread_params[i].init_done == 1)
                {
                    if (_thread_params[i].init_fail == 1)
                    {
                        init_fail = true;
                        break;
                    }
                    else
                    {
                        ++done_num;
                    }
                }
            }
            if (init_fail || done_num == _thread_num)
            {
                break;
            }
            usleep(100000);
        }
        if (init_fail)
        {
            //SLOG(ERROR, "start(): start thread group [%s] failed", _name.c_str());
            stop();
            return false;
        }
        //SLOG(INFO, "start(): thread group [%s] started, thread_num=%d", _name.c_str(), _thread_num);
        return true;
    }

    void stop()
    {
        if (!_is_running) return;
        _is_running = false;

        delete _io_service_work;
        _io_service_work = NULL;

        for (int i = 0; i < _thread_num; ++i)
        {
            int ret = pthread_join(_threads[i], NULL);
            if (ret != 0)
            {
                //SLOG(ERROR, "stop(): join thread[%d] failed: error=%d", i, ret);
            }
        }

        delete []_thread_params;
        _thread_params = NULL;
        delete []_threads;
        _threads = NULL;

        //SLOG(INFO, "stop(): thread group [%s] stopped", _name.c_str());
    }

    // Request the thread group to invoke the given handler.
    // The handler may be executed inside this function if the guarantee can be met.
    // The function signature of the handler must be:
    //   void handler();
    template< typename CompletionHandler >
    void dispatch(CompletionHandler handler)
    {
        _io_service.dispatch(handler);
    }

    // Request the thread group to invoke the given handler and return immediately.
    // It guarantees that the handle will not be called from inside this function.
    // The function signature of the handler must be:
    //   void handler();
    template< typename CompletionHandler >
    void post(CompletionHandler handler)
    {
        _io_service.post(handler);
    }

    void dispatch(google::protobuf::Closure* handle)
    {
        dispatch(boost::bind(&ThreadGroupImpl::closure_run_helper, handle));
    }

    void post(google::protobuf::Closure* handle)
    {
        post(boost::bind(&closure_run_helper, handle));
    }

    void dispatch(ExtClosure<void()>* handle)
    {
        dispatch(boost::bind(&ThreadGroupImpl::ext_closure_run_helper, handle));
    }

    void post(ExtClosure<void()>* handle)
    {
        post(boost::bind(&ext_closure_run_helper, handle));
    }

private:
    static void* thread_run(void* param)
    {
        ThreadParam* thread_param = reinterpret_cast<ThreadParam*>(param);
        // init
        if (thread_param->init_func && !thread_param->init_func->Run())
        {
            //SLOG(ERROR, "thread_run(): init thread [%d] failed", thread_param->id);
            ++thread_param->init_fail;
        }
        ++thread_param->init_done;
        // run asio
        if (thread_param->init_fail == 0)
        {
            thread_param->io_service->run();
        }
        // destroy
        if (thread_param->dest_func)
        {
            thread_param->dest_func->Run();
        }
        return NULL;
    }

    static void closure_run_helper(google::protobuf::Closure* handle)
    {
        handle->Run();
    }

    static void ext_closure_run_helper(ExtClosure<void()>* handle)
    {
        handle->Run();
    }

private:
    volatile bool _is_running;
    int _thread_num;
    std::string _name;
    ThreadInitFunc _init_func;
    ThreadDestFunc _dest_func;

    IOService _io_service;
    IOServiceWork* _io_service_work;
    pthread_t* _threads;
    ThreadParam* _thread_params;

    //DISALLOW_EVIL_CONSTRUCTORS(ThreadGroupImpl);
};

} // 
} //

#endif // 
