// Copyright (c) 2014 Baidu.com, Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMMON_INTERNAL_H_
#define COMMON_INTERNAL_H_

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <memory>

//#include <hdcs/networking/common.h>
#include "atomic.h"
#include "counter.h"
#include "../controller/controller_impl.h"
//#include <hdcs/networking/func_tracer.h>
//#include <hdcs/networking/io_service.h>
//#include <hdcs/networking/locks.h>
//#include <hdcs/networking/ptime.h>

namespace hdcs {
namespace networking {

class ControllerImpl;
typedef std::shared_ptr<ControllerImpl> RpcControllerImplPtr;
typedef hdcs::networking::weak_ptr<RpcControllerImpl> RpcControllerImplWPtr;
/*
class RpcChannelImpl;
typedef hdcs::networking::shared_ptr<RpcChannelImpl> RpcChannelImplPtr;

class RpcClientImpl;
typedef hdcs::networking::shared_ptr<RpcClientImpl> RpcClientImplPtr;

class RpcServerImpl;
typedef hdcs::networking::shared_ptr<RpcServerImpl> RpcServerImplPtr;
typedef hdcs::networking::weak_ptr<RpcServerImpl> RpcServerImplWPtr;

class RpcClientStream;
typedef hdcs::networking::shared_ptr<RpcClientStream> RpcClientStreamPtr;
typedef hdcs::networking::weak_ptr<RpcClientStream> RpcClientStreamWPtr;

class RpcServerStream;
typedef hdcs::networking::shared_ptr<RpcServerStream> RpcServerStreamPtr;
typedef hdcs::networking::weak_ptr<RpcServerStream> RpcServerStreamWPtr;

class RpcListener;
typedef hdcs::networking::shared_ptr<RpcListener> RpcListenerPtr;

class TimerWorker;
typedef hdcs::networking::shared_ptr<TimerWorker> TimerWorkerPtr;

class RpcTimeoutManager;
typedef hdcs::networking::shared_ptr<RpcTimeoutManager> RpcTimeoutManagerPtr;

class ThreadGroup;
typedef hdcs::networking::shared_ptr<ThreadGroup> ThreadGroupPtr;

class ServicePool;
typedef hdcs::networking::shared_ptr<ServicePool> ServicePoolPtr;
typedef hdcs::networking::weak_ptr<ServicePool> ServicePoolWPtr;

class FlowController;
typedef hdcs::networking::shared_ptr<FlowController> FlowControllerPtr;

class WaitEvent;
typedef hdcs::networking::shared_ptr<WaitEvent> WaitEventPtr;

class IOServicePool;
typedef hdcs::networking::shared_ptr<IOServicePool> IOServicePoolPtr;

class WebService;
typedef hdcs::networking::shared_ptr<WebService> WebServicePtr;
#define SOFA_PBRPC_DECLARE_RESOURCE_COUNTER(name_) \
    extern hdcs::networking::AtomicCounter g_hdcs_counter_##name_
#define SOFA_PBRPC_DEFINE_RESOURCE_COUNTER(name_) \
    hdcs::networking::AtomicCounter g_hdcs_counter_##name_(0)
#define SOFA_PBRPC_INC_RESOURCE_COUNTER(name_) \
    ++g_hdcs_counter_##name_
#define SOFA_PBRPC_DEC_RESOURCE_COUNTER(name_) \
    --g_hdcs_counter_##name_
#define SOFA_PBRPC_GET_RESOURCE_COUNTER(name_) \
    static_cast<int>(g_hdcs_counter_##name_)

//SOFA_PBRPC_DECLARE_RESOURCE_COUNTER(RpcByteStream);
//SOFA_PBRPC_DECLARE_RESOURCE_COUNTER(RpcListener);

// Use for affecting global/static variables' construct/destruct order.
inline void touch_boost_error_category()
{
    (void)boost::system::system_category();
    (void)boost::system::generic_category();
    (void)boost::asio::error::get_addrinfo_category();
    (void)boost::asio::error::get_misc_category();
    (void)boost::asio::error::get_netdb_category();
}
*/

} 
}

#endif
