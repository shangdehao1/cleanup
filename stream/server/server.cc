#include <signal.h>
#include <unistd.h>

//#include "server.h"
#include "server_impl.h"
#include "server.h"

#include "../../common/smart_ptr/networking_ptr.h"

namespace hdcs {
namespace networking {

Server::Server(const ServerOptions& options, 
                     EventHandler* handler)
{
    //touch_boost_error_category(); // TODO 
    _impl.reset(new ServerImpl(options, handler)); // !!
}

Server::~Server()
{
    Stop();
}

bool Server::Start(const std::string& server_address)
{
    return _impl->Start(server_address);
}

void Server::Stop()
{
    _impl->Stop();
}

static volatile bool s_quit = false;
static void SignalIntHandler(int )
{
    s_quit = true;
}
int Server::Run()
{
    signal(SIGINT, SignalIntHandler);
    signal(SIGTERM, SignalIntHandler);
    while (!s_quit) {
        sleep(1);
    }
    return 0;
}

ServerOptions Server::GetOptions()
{
    return _impl->GetOptions();
}

void Server::ResetOptions(const ServerOptions& options)
{
    _impl->ResetOptions(options);
}

bool Server::RegisterService(google::protobuf::Service* service, bool take_ownership)
{
    return _impl->RegisterService(service, take_ownership);
}

int Server::ServiceCount()
{
    return _impl->ServiceCount();
}

int Server::ConnectionCount()
{
    return _impl->ConnectionCount();
}

bool Server::IsListening()
{
    return _impl->IsListening();
}

}
} 
