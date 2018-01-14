#ifndef _SOFA_PBRPC_RPC_BYTE_STREAM_H_
#define _SOFA_PBRPC_RPC_BYTE_STREAM_H_

#include <cstdio> // for snprintf()
#include <cstring> // for memset()

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "../common/atomic.h"
#include "../common/smart_ptr/networking_ptr.h"
#include "endpoint.h"

// If SOFA_PBRPC_TCP_NO_DELAY == true, means disable the Nagle algorithm.
//
// Nagle algorithm may cause an extra delay in some cases, because if
// the data in a single write spans 2n packets, the last packet will be
// withheld, waiting for the ACK for the previous packet. For more, please
// refer to <https://en.wikipedia.org/wiki/Nagle's_algorithm>.
//
// Disabling the Nagle algorithm would cause these affacts:
//   * decrease delay time (positive affact)
//   * decrease the qps (negative affact)
#ifndef TCP_NO_DELAY
#define TCP_NO_DELAY true
#endif

// tcp status

namespace hdcs {
namespace networking {

using boost::asio::ip::tcp;
using namespace boost::asio;

class ByteStream : public hdcs::networking::enable_shared_from_this<ByteStream>
{
public:
    ByteStream(IOService& io_service, const Endpoint& endpoint)
        : _io_service(io_service)
        , _remote_endpoint(endpoint)
        , _ticks(0)
        , _last_rw_ticks(0)
        , _timer(io_service)
        , _socket(io_service)
        , _connect_timeout(-1)
        , _status(STATUS_INIT){
        memset(_error_message, 0, sizeof(_error_message));
    }

    virtual ~ByteStream(){
        boost::system::error_code ec;
        _socket.close(ec);
    }

    void close(const std::string& reason){
        // should run for only once
        if (atomic_swap(&_status, (int)STATUS_CLOSED) != STATUS_CLOSED)
        {
            snprintf(_error_message, sizeof(_error_message), "%s", reason.c_str());
            boost::system::error_code ec;
            _socket.shutdown(tcp::socket::shutdown_both, ec);
            on_closed();
            if (_remote_endpoint != Endpoint())
            {
                //SLOG(INFO, "close(): connection closed: %s: %s",
                 //       EndpointToString(_remote_endpoint).c_str(), _error_message);
            }
        }
    }

    // callback for connection timeout.
    void on_connect_timeout(const boost::system::error_code& error) 
    {
        if (_status != STATUS_CONNECTING) 
        {
            return;
        }
        if (error == boost::asio::error::operation_aborted) 
        {
            return;
        }
        close("connect timeout");
    }

    // Connect the channel. Used by client.
    void async_connect()
    {
        _last_rw_ticks = _ticks;
        _status = STATUS_CONNECTING;
        _socket.async_connect(_remote_endpoint,
                boost::bind(&ByteStream::on_connect, shared_from_this(), _1));
        if (_connect_timeout > 0) 
        {
            _timer.expires_from_now(boost::posix_time::milliseconds(_connect_timeout));
            _timer.async_wait(boost::bind(&ByteStream::on_connect_timeout, shared_from_this(), _1));
        }
    }

    // Update remote endpoint from socket.  Used by server.
    // Precondition:
    // * the "socket" is opened.
    void update_remote_endpoint()
    {
        boost::system::error_code ec;
        _remote_endpoint = _socket.remote_endpoint(ec);
        if (ec)
        {
            //SLOG(ERROR, "update_remote_endpoint(): get remote endpoint failed: %s",
             //       ec.message().c_str());
            close("init stream failed: " + ec.message());
        }
    }

    // Set socket connected.  Used by server.
    // Precondition:
    // * the "socket" is opened.
    void set_socket_connected()
    {
        //SOFA_PBRPC_FUNCTION_TRACE;
        _last_rw_ticks = _ticks;
        boost::system::error_code ec;
        _socket.set_option(tcp::no_delay(TCP_NO_DELAY), ec);
        if (ec)
        {
         //   SLOG(ERROR, "set_socket_connected(): set no_delay option failed: %s",
         //           ec.message().c_str());
            close("init stream failed: " + ec.message());
            return;
        }

        _local_endpoint = _socket.local_endpoint(ec);
        if (ec)
        {
           //  SLOG(ERROR, "set_socket_connected(): get local endpoint failed: %s",
           //         ec.message().c_str());
            close("init stream failed: " + ec.message());
            return;
        }

        if (!on_connected())  // pure virtual function.
        {
            //SLOG(ERROR, "set_socket_connected(): call on_connected() failed");
            close("init stream failed: call on_connected() failed");
            return;
        }
        _status = STATUS_CONNECTED;
        trigger_receive();
        trigger_send();
    }

    // Get the socket.
    tcp::socket& socket()
    {
        return _socket;
    }

    // Get the local endpoint.
    const Endpoint& local_endpoint() const
    {
        return _local_endpoint;
    }

    // Get the remote endpoint.
    const Endpoint& remote_endpoint() const
    {
        return _remote_endpoint;
    }

    // Check if the channel is connecting.
    bool is_connecting() const
    {
        return _status == STATUS_CONNECTING;
    }

    // Check if the channel is connected.
    bool is_connected() const
    {
        return _status == STATUS_CONNECTED;
    }

    // Check if the channel is closed.
    bool is_closed() const
    {
        return _status == STATUS_CLOSED;
    }

    // Reset current time ticks.
    void reset_ticks(int64_t ticks, bool update_last_rw_ticks)
    {
        _ticks = ticks;
        if (update_last_rw_ticks)
        {
            _last_rw_ticks = ticks;
        }
    }

    // Get the last time ticks for read or write.
    int64_t last_rw_ticks() const
    {
        return _last_rw_ticks;
    }

    void set_connect_timeout(int64_t timeout) 
    {
        _connect_timeout = timeout;
    }

    int64_t connect_timeout() 
    {
        return _connect_timeout;
    }

    // Trigger receiving operator.
    // @return true if suceessfully triggered
    virtual bool trigger_receive() = 0;

    // Trigger sending operator.
    // @return true if suceessfully triggered
    virtual bool trigger_send() = 0;

protected:

    // Async read some data from the stream.
    void async_read_some(char* data, size_t size)
    {
        _socket.async_read_some(boost::asio::buffer(data, size),
                boost::bind(&ByteStream::on_read_some, 
                    shared_from_this(), _1, _2));
    }

    // Async write some data to the stream.
    void async_write_some(const char* data, size_t size)
    {
        _socket.async_write_some(boost::asio::buffer(data, size),
                boost::bind(&ByteStream::on_write_some, 
                    shared_from_this(), _1, _2));
    }

    // Hook function when connected.
    // @return false if some error occured.
    virtual bool on_connected() = 0;

    // Callback of "async_read_some()".
    virtual void on_read_some(
            const boost::system::error_code& error,
            std::size_t bytes_transferred) = 0;

    // Callback of "async_write_some()".
    virtual void on_write_some(
            const boost::system::error_code& error,
            std::size_t bytes_transferred) = 0;

    // when implement this function
    // Hook function when closed.
    virtual void on_closed() = 0;


private:

    // Callback of "async_connect()".
    void on_connect(const boost::system::error_code& error)
    {
        //Maybe already timeout
        if (_status != STATUS_CONNECTING) 
        {
            return;
        }

        if (error)
        {
            // TODO retry connect?
            //SLOG(ERROR, "on_connect(): connect error: %s: %s",
            //       EndpointToString(_remote_endpoint).c_str(),
            //      error.message().c_str());
            close("init stream failed: " + error.message());
            return;
        }

        boost::system::error_code ec;
        _socket.set_option(tcp::no_delay(TCP_NO_DELAY), ec);
        if (ec)
        {
            //SLOG(ERROR, "on_connect(): set no_delay option failed: %s",
            //       ec.message().c_str());
            close("init stream failed: " + ec.message());
            return;
        }

        _local_endpoint = _socket.local_endpoint(ec);
        if (ec)
        {
            //SLOG(ERROR, "on_connect(): get local endpoint failed: %s",
            //       ec.message().c_str());
            close("init stream failed: " + ec.message());
            return;
        }

        if (!on_connected()) // callback 
        {
            //SLOG(ERROR, "on_connect(): call on_connected() failed");
            close("init stream failed: call on_connected() failed");
            return;
        }
        //SLOG(INFO, "on_connect(): connection established: %s",
        //       EndpointToString(_remote_endpoint).c_str());

        _status = STATUS_CONNECTED;
        _timer.cancel();
        trigger_receive();
        trigger_send();
    }

protected:
    IOService& _io_service;
    Endpoint _local_endpoint;
    Endpoint _remote_endpoint;
    char _error_message[128];
    volatile int64_t _ticks;
    volatile int64_t _last_rw_ticks;
private:
    deadline_timer _timer;
    tcp::socket _socket;
    int64_t _connect_timeout;
    enum {
        STATUS_INIT       = 0,
        STATUS_CONNECTING = 1,
        STATUS_CONNECTED  = 2,
        STATUS_CLOSED     = 3,
    };
    volatile int _status;
};
}
}
#endif
