#ifndef SIMPLE_CHANNEL_IMPL_H_
#define SIMPLE_CHANNEL_IMPL_H_

#include <google/protobuf/service.h>

#include "channel/channel_impl.h"
#include "stream/client/client_impl.h"


namespace hdcs {
namespace networking {

class SimpleChannelImpl : public ChannelImpl,
    public ::hdcs::networking::enable_shared_from_this<SimpleChannelImpl>
{
public:
    SimpleChannelImpl(const ClientImplPtr& rpc_client_impl,
                         const std::string& server_address,
                         const ChannelOptions& options);

    virtual ~SimpleChannelImpl();

    virtual bool Init();

    virtual void Stop();

    virtual void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                            ::google::protobuf::RpcController* controller,
                            const ::google::protobuf::Message* request,
                            ::google::protobuf::Message* response,
                            ::google::protobuf::Closure* done);

    virtual uint32_t WaitCount();

private:
    static void WaitDone(const ControllerImplPtr& cntl);

    void DoneCallback(google::protobuf::Closure* done,
                      const ControllerImplPtr& cntl);

    static void MockDoneCallback(ControllerImplPtr cntl,
                                 const ::google::protobuf::Message* request,
                                 ::google::protobuf::Message* response);

private:
    ClientImplPtr _client_impl;
    std::string _server_address;
    ChannelOptions _options;

    bool _is_mock;
    bool _resolve_address_succeed;
    Endpoint _remote_endpoint;

    AtomicCounter _wait_count;

}; // class SimpleChannelImpl

} // namespace networking
} // namespace hdcs

#endif
