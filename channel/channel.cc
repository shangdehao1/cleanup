
#include <sstream>

/*
#include <sofa/pbrpc/rpc_channel.h>
#include <sofa/pbrpc/simple_rpc_channel_impl.h>
#include <sofa/pbrpc/dynamic_rpc_channel_impl.h>
#include <sofa/pbrpc/rpc_client.h>
*/

namespace hdcs {
namespace networking {

Channel::Channel(Client* client,
        const std::string& server_address,
        const ChannelOptions& options)
    : _impl(new SimpleChannelImpl(client->impl(), server_address, options)) // !!
{
    if (options.create_with_init)
    {
        _impl->Init();
    }
}

Channel::Channel(Client* client,
        const std::string& server_ip,
        uint32 server_port,
        const ChannelOptions& options)
{
    std::ostringstream os;
    os << server_ip << ":" << server_port;
    _impl.reset(new SimpleChannelImpl(client->impl(), os.str(), options)); //!!
    if (options.create_with_init)
    {
        _impl->Init();
    }
}

Channel::Channel(Client* client,
        const std::vector<std::string>& address_list,
        const ChannelOptions& options)
    : _impl(new DynamicChannelImpl(client->impl(), address_list, options)) //!!
{
    if (options.create_with_init)
    {
        _impl->Init();
    }
}

Channel::Channel(Client* client,
        AddressProvider* address_provider,
        const ChannelOptions& options)
    : _impl(new DynamicChannelImpl(client->impl(), address_provider, options))
{
    if (options.create_with_init)
    {
        _impl->Init();
    }
}

bool Channel::Init()
{
    return _impl->Init();
}

Channel::~Channel()
{
    _impl->Stop();
}

void Channel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                            ::google::protobuf::Controller* controller,
                            const ::google::protobuf::Message* request,
                            ::google::protobuf::Message* response,
                            ::google::protobuf::Closure* done)
{
    _impl->CallMethod(method, controller, request, response, done);
}

} // namespace networking
} // namespace hdcs
