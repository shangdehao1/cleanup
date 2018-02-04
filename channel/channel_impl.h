#ifndef CHANNEL_IMPL_H_
#define CHANNEL_IMPL_H_

namespace hdcs {
namespace networking {

// pure virtual class, namely interface class
class ChannelImpl : public ::google::protobuf::RpcChannel
{
public:
    virtual ~ChannelImpl() {}

    // Init the channel.
    virtual bool Init() = 0;

    // Stop the channel.
    virtual void Stop() = 0;

    // Call method.
    virtual void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                            ::google::protobuf::RpcController* controller,
                            const ::google::protobuf::Message* request,
                            ::google::protobuf::Message* response,
                            ::google::protobuf::Closure* done) = 0;

    // Get the count of calls which are not done yet.
    virtual uint32_t WaitCount() = 0;
};

} // namespace netwokring
} // namespace hdcs 

#endif
