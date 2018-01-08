#ifndef NAMESPACE_H
#define NAMESPACE_H


#include "smart_ptr/networking_ptr.h"

namespace hdcs{
namespace networking{

class ClientStream;
typedef hdcs::networking::shared_ptr<ClientStream> ClientStreamPtr;

class ServerStream;
typedef hdcs::networking::shared_ptr<ServerStream> ServerStreamPtr;


};
};



















#endif
