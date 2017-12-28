// Copyright (c) 2014 Baidu.com, Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _SOFA_PBRPC_RPC_LISTENER_H_
#define _SOFA_PBRPC_RPC_LISTENER_H_

#include <sofa/pbrpc/common_internal.h>
#include <sofa/pbrpc/rpc_endpoint.h>
#include <sofa/pbrpc/rpc_error_code.h>
#include <sofa/pbrpc/locks.h>
#include <sofa/pbrpc/io_service_pool.h>

namespace sofa {
namespace pbrpc {

using boost::asio::ip::tcp;

class Listener : public sofa::pbrpc::enable_shared_from_this<Listener>
{
    // Callback function when created or accepted a new connection.
    typedef boost::function<void(const RpcServerStreamPtr& /* stream */)> Callback;
    typedef boost::function<void(RpcErrorCode /* error_code */,
            const std::string& /* error_text */)> FailCallback;

public:
    static const int LISTEN_MAX_CONNECTIONS = 4096;
    
public:
    Listener(IOServicePoolPtr& io_service_pool, const RpcEndpoint& endpoint)
        : _io_service_pool(io_service_pool)
        , _endpoint(endpoint)
        , _endpoint_str(RpcEndpointToString(endpoint))
        , _acceptor(io_service_pool->GetIOService()) // the only one acceptor
        , _is_closed(true)
    {
        SOFA_PBRPC_INC_RESOURCE_COUNTER(Listener);
    }

    virtual ~Listener()
    {
        SOFA_PBRPC_FUNCTION_TRACE;
        close();
        SOFA_PBRPC_DEC_RESOURCE_COUNTER(Listener);
    }

    void close()
    {
        ScopedLocker<MutexLock> _(_close_lock);
        if (_is_closed) return;
        _is_closed = true;

        boost::system::error_code ec;
        _acceptor.cancel(ec);
        _acceptor.close(ec);

        SLOG(INFO, "close(): listener closed: %s", _endpoint_str.c_str());
    }

    bool is_closed()
    {
        ScopedLocker<MutexLock> _(_close_lock);
        return _is_closed;
    }

    // Set the callback funtion when created a new connection.
    template <typename T>
    void set_create_callback(const T& create_callback)
    {
        _create_callback = create_callback;
    }

    // Set the callback funtion when accepted a new connection.
    template <typename T>
    void set_accept_callback(const T& accept_callback)
    {
        _accept_callback = accept_callback;
    }

    // Set the callback funtion when failed to accept connection.
    template <typename T>
    void set_accept_fail_callback(const T& accept_fail_callback)
    {
        _accept_fail_callback = accept_fail_callback;
    }

    // Start listen.  Return false if failed.
    bool start_listen()
    {
        boost::system::error_code ec;

        _acceptor.open(_endpoint.protocol(), ec);
        if (ec)
        {
            SLOG(ERROR, "start_listen(): open acceptor failed: %s: %s",
                    _endpoint_str.c_str(), ec.message().c_str());
            return false;
        }

        int ret = fcntl(_acceptor.native(), F_SETFD, 
                        fcntl(_acceptor.native(), F_GETFD) | FD_CLOEXEC);
        if (ret < 0)
        {
            SLOG(ERROR, "start_listen(): make fd close_on_exec failed: %s: %s",
                    _endpoint_str.c_str(), strerror(errno));
            return false;
        }

        _acceptor.set_option(tcp::acceptor::reuse_address(true), ec);
        if (ec)
        {
            SLOG(ERROR, "start_listen(): set acceptor option failed: %s: %s",
                    _endpoint_str.c_str(), ec.message().c_str());
            return false;
        }

        _acceptor.bind(_endpoint, ec);
        if (ec)
        {
            SLOG(ERROR, "start_listen(): bind acceptor failed: %s: %s",
                    _endpoint_str.c_str(), ec.message().c_str());
            return false;
        }

        //_acceptor.listen(boost::asio::socket_base::max_connections, ec);
        _acceptor.listen(LISTEN_MAX_CONNECTIONS, ec);
        if (ec)
        {
            SLOG(ERROR, "start_listen(): listen acceptor failed: %s: %s",
                    _endpoint_str.c_str(), ec.message().c_str());
            return false;
        }

        SLOG(INFO, "start_listen(): listen succeed: %s", _endpoint_str.c_str());
        _is_closed = false;

        // async accept operation.
        async_accept(); 

        return true;
    }

private:

    void async_accept()
    {
        // one connection, one rpc_server_stream.
        RpcServerStreamPtr stream(new RpcServerStream(_io_service_pool->GetIOService()));

        // callback for create stream
        if (_create_callback)
            _create_callback(stream);

        _acceptor.async_accept(stream->socket(), boost::bind(
                    &Listener::on_accept, shared_from_this(), stream, _1));
    }

    // callback for async_connection.
    void on_accept(const RpcServerStreamPtr& stream,
            const boost::system::error_code& ec)
    {
        if (_is_closed)
            return;

        if (ec)
        {
            SLOG(ERROR, "on_accept(): accept error at %s: %s",
                    _endpoint_str.c_str(), ec.message().c_str());

            close();

            if (_accept_fail_callback)
            {
                RpcErrorCode error_code = ec == boost::asio::error::no_descriptors ?
                    RPC_ERROR_TOO_MANY_OPEN_FILES : RPC_ERROR_UNKNOWN;
                 // calllback for fail connection.
                _accept_fail_callback(error_code, ec.message());
            }
        }
        else
        {
            stream->update_remote_endpoint();

            if (!stream->is_closed() && _accept_callback)
            {
                _accept_callback(stream);
            }

            if (!stream->is_closed())
            {
                stream->set_socket_connected();
            }

            if (!stream->is_closed())
            {
                SLOG(INFO, "on_accept(): accept connection at %s: %s",
                        _endpoint_str.c_str(), RpcEndpointToString(stream->remote_endpoint()).c_str());
            }

            async_accept(); // listen next stream
        }
    }

private:
    IOServicePoolPtr& _io_service_pool;
    RpcEndpoint _endpoint;
    std::string _endpoint_str;
    // the following three callback function will be set by server_impl.
    Callback _create_callback;
    Callback _accept_callback;
    FailCallback _accept_fail_callback;
    tcp::acceptor _acceptor;
    volatile bool _is_closed;
    MutexLock _close_lock;

    SOFA_PBRPC_DISALLOW_EVIL_CONSTRUCTORS(Listener);
}; // class Listener

} // namespace pbrpc
} // namespace sofa

#endif // _SOFA_PBRPC_RPC_LISTENER_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
