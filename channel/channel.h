#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <vector>
#include <string>

#include <google/protobuf/service.h>

#include "stream/endpoint.h"
#include "common/smart_ptr/networking_ptr.h"


namespace hdcs {
namespace networking {

// Defined in other files.
class Client;
class ChannelImpl;

struct ChannelOptions {
    // Connect timeout (in seconds).
    // If a request can't get an healthy connection after a connect_timeout
    // milliseconds, it will fail and return to the caller.
    //
    // Not supported now.
    int64_t connect_timeout;

    //////////// The following options are only useful for multiple servers. //////////
    // Server load capacity, which limits the max request count pending on one channel.
    // If all servers exceed the limit, calling will returns RPC_ERROR_SERVER_BUSY.
    // Value 0 means no limit, default value is 0.
    uint32_t server_load_capacity;

    // If initialize the Channel in construct function, default is true.
    // If create_with_init is false, Channel should be initialized by calling Init().
    bool create_with_init;

    ChannelOptions()
        : connect_timeout(10)
        , server_load_capacity(0)
        , create_with_init(true)
    {}
};

class Channel : public google::protobuf::RpcChannel
{
public:
//  ================================== EventHandler ==========================
    class EventHandler {
    public:
        virtual ~EventHandler() {}
        
        // This method is called when some addresses added.
        virtual void OnAddressAdded(const std::vector<std::string>& address_list) = 0;

        // This method is called when some addresses removed.
        virtual void OnAddressRemoved(const std::vector<std::string>& address_list) = 0;
    };

//  ================================= AddressProvider ========================
    class AddressProvider {
    public:
        virtual ~AddressProvider() {}

        // Get the init address list.
        // This will be called for once when create the channel.
        virtual void GetInitAddress(std::vector<std::string>* address_list) = 0;

        // Register an event handler.
        // The "event_handler" is take ownership by the address provider.
        virtual bool RegisterEventHandler(EventHandler* event_handler) = 0;
    };

public:
    // Create single server point by server address.
    // The "client" is owned by the caller.
    // The "server_address" should be in format of "ip:port".
    Channel(Client* client,
               const std::string& server_address,
               const ChannelOptions& options = ChannelOptions());

    // Create single server point by server ip and port.
    // The "client" is owned by the caller.
    Channel(Client* client,
               const std::string& server_ip,
               uint32_t server_port,
               const ChannelOptions& options = ChannelOptions());

    // Create multiple server points by server address list.
    // The "client" is owned by the caller.
    Channel(Client* client,
               const std::vector<std::string>& address_list,
               const ChannelOptions& options = ChannelOptions());

    // Create multiple server points by address provider.
    // The "client" is owned by the caller.
    // The "address_provider" is owned by the caller.
    Channel(Client* client,
               AddressProvider* address_provider,
               const ChannelOptions& options = ChannelOptions());

    // Initialize Channel.
    // For single server point, it will resolve server address in this function,
    // and if resolve server address succeed return true, otherwise return false.
    // For multiple server points, it will update internal server list and
    // register detect task. After all of these are completed return true, and
    // never return false.
    bool Init();

    // Destructor.
    virtual ~Channel();

    // Implements the google::protobuf::Channel interface.  If the
    // "done" is NULL, it's a synchronous call, or it's asynchronous and
    // uses the callback to inform the completion (or failure). 
    virtual void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                            ::google::protobuf::RpcController* controller,
                            const ::google::protobuf::Message* request,
                            ::google::protobuf::Message* response,
                            ::google::protobuf::Closure* done); //

public:
    const hdcs::networking::shared_ptr<ChannelImpl>& impl() const
    {
        return _impl;
    }

private:
    hdcs::networking::shared_ptr<ChannelImpl> _impl;

}; // class Channel

} // namespace networking 
} // namespace hdcs 

#endif
