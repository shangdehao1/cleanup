#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <sstream>
#include <string>
#include <boost/asio.hpp>

namespace hdcs {
namespace networking {

typedef boost::asio::ip::tcp::endpoint Endpoint;
typedef boost::asio::io_service IOService;

using boost::asio::ip::tcp;

std::string EndPointToString(const Endpoint& endpoint){
    std::stringstream ss;
    ss << endpoint;
    return ss.str();
}

std::string HostOfEndpoint(const Endpoint& endpoint){
    return endpoint.address().to_string();
}

uint32_t PortOfEndpoint(const Endpoint& endpoint){
    return endpoint.port();
}

bool ResolveAddress(IOService& io_service,
        const std::string& host, const std::string& svc, Endpoint* endpoint){
    tcp::resolver resolver(io_service);
    boost::system::error_code ec;
    tcp::resolver::iterator it = resolver.resolve(tcp::resolver::query(host, svc), ec), end;
    if (it != end){
        *endpoint = it->endpoint();
        return true;
    }else{
        std::cout<<"resolveAddress: resolve address failed: "<<ec.message()<<std::endl;
        return false;
    }
}

bool ResolveAddress(IOService& io_service,
        const std::string& address,
        Endpoint* endpoint)
{
    std::string::size_type pos = address.find(':');
    if (pos == std::string::npos)
    {
        std::cout<<"ResolveAddress(): invalid address"<<std::endl;
        return false;
    }
    std::string host = address.substr(0, pos);
    std::string svc = address.substr(pos + 1);
    return ResolveAddress(io_service, host, svc, endpoint);
}

} 
}
#endif
