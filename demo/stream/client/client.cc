#include "client.h"
#include "client_impl.h"

namespace hdcs {
namespace networking {

Client::Client(const ClientOptions& options)
{
    touch_boost_error_category();
    _impl.reset(new ClientImpl(options));
    _impl->Start();
}

Client::~Client()
{
    Shutdown();
}

ClientOptions Client::GetOptions()
{
    return _impl->GetOptions();
}

void Client::ResetOptions(const ClientOptions& options)
{
    _impl->ResetOptions(options);
}

int Client::ConnectionCount()
{
    return _impl->ConnectionCount();
}

void Client::Shutdown()
{
    _impl->Stop();
}

} // 
} // 
