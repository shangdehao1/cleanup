#ifndef IO_SERVICE_H_
#define IO_SERVICE_H_

 // ATTENTION: suppose that epoll is always supported on the platform.

#if __APPLE__
#undef BOOST_ASIO_HAS_EPOLL
#else
#define BOOST_ASIO_HAS_EPOLL 1
#endif

#include <boost/asio.hpp>
#include "../common/smart_ptr/networking_ptr.h"

namespace hdcs {
namespace networking {

typedef boost::asio::io_service IOService;
typedef hdcs::networking::shared_ptr<IOService> IOServicePtr;

typedef boost::asio::io_service::work IOServiceWork;
typedef hdcs::networking::shared_ptr<IOServiceWork> IOServiceWorkPtr;

typedef boost::asio::io_service::strand IOServiceStrand;
typedef hdcs::networking::shared_ptr<IOServiceStrand> IOServiceStrandPtr;

typedef boost::asio::deadline_timer IOServiceTimer;
typedef hdcs::networking::shared_ptr<IOServiceTimer> IOServiceTimerPtr;

}  
}  

#endif
