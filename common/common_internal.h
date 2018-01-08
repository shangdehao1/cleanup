// Copyright (c) 2014 Baidu.com, Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMMON_INTERNAL_H_
#define COMMON_INTERNAL_H_

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <memory>

#include "./smart_ptr/networking_ptr.h"

namespace hdcs {
namespace networking {

class ControllerImpl;
typedef hdcs::networking::shared_ptr<ControllerImpl> ControllerImplPtr;
typedef hdcs::networking::weak_ptr<ControllerImpl> ControllerImplWPtr;

class ChannelImpl;
typedef hdcs::networking::shared_ptr<ChannelImpl> ChannelImplPtr;

class ClientImpl;
typedef hdcs::networking::shared_ptr<ClientImpl> ClientImplPtr;

class ServerImpl;
typedef hdcs::networking::shared_ptr<ServerImpl> ServerImplPtr;
typedef hdcs::networking::weak_ptr<ServerImpl> ServerImplWPtr;

class ClientStream;
typedef hdcs::networking::shared_ptr<ClientStream> ClientStreamPtr;
typedef hdcs::networking::weak_ptr<ClientStream> ClientStreamWPtr;

class ServerStream;
typedef hdcs::networking::shared_ptr<ServerStream> ServerStreamPtr;
typedef hdcs::networking::weak_ptr<ServerStream> ServerStreamWPtr;

class Listener;
typedef hdcs::networking::shared_ptr<Listener> ListenerPtr;

class TimerWorker;
typedef hdcs::networking::shared_ptr<TimerWorker> TimerWorkerPtr;

class TimeoutManager;
typedef hdcs::networking::shared_ptr<TimeoutManager> TimeoutManagerPtr;

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

/*
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

//SOFA_PBRPC_DECLARE_RESOURCE_COUNTER(ByteStream);
//SOFA_PBRPC_DECLARE_RESOURCE_COUNTER(Listener);

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
