#ifndef SERVER_STREAM_H_
#define SERVER_STREAM_H_

#include <functional>

#include <boost/smart_ptr.hpp>

#include "../../buffer/buffer.h"
#include "../../data/request/request.h"
#include "../../common/smart_ptr/networking_ptr.h"
#include "message_stream.h"


namespace hdcs {
namespace networking {

// define some callback function 
// Callback function when received request message.
typedef std::function<void(
        const ServerStreamPtr& ,
        const RequestPtr& )> ReceivedRequestCallback;

// Callback function when closed server stream.
typedef std::function<void(
        const ServerStreamPtr& )> ClosedServerStreamCallback;

// Callback function when send response message done.
//  * if "status" == HDCS_NETWORK_SUCCESS, means send response succeed;
//  * else, means send failed.
typedef std::function<void(
        ErrorCode )> SendResponseCallback;

class ServerStream : public ServerMessageStream<SendResponseCallback>
{
public:
    ServerStream(IOService& io_service)
        : ServerMessageStream<SendResponseCallback>(
                ROLE_TYPE_SERVER, io_service, Endpoint())
    {}

    virtual ~ServerStream() 
    {
        //FUNCTION_TRACE;
    }

    // Set the callback function when received request.
    void set_received_request_callback(
            const ReceivedRequestCallback& callback)
    {
        _received_request_callback = callback;
    }

    // Get the callback function when received request.
    const ReceivedRequestCallback& received_request_callback() const
    {
        return _received_request_callback;
    }

    // Set the callback function when closed stream.
    void set_closed_stream_callback(
            const ClosedServerStreamCallback& callback)
    {
        _closed_stream_callback = callback;
    }

    // Get the callback function when closed stream.
    const ClosedServerStreamCallback& closed_stream_callback() const
    {
        return _closed_stream_callback;
    }

    // Increase pending process count.
    void increase_pending_process_count()
    {
        ++_pending_process_count;
    }

    // Decrease pending process count.
    void decrease_pending_process_count()
    {
        --_pending_process_count;
    }

    // Get pending process count.
    uint32_t pending_process_count() const
    {
        return _pending_process_count;
    }

    // Send response message.  No timeout is set for sending response.
    // If send done, on mater succeed or failed, SendResponseCallback will be called.
    // @param message  the response message to send, including the header.  not null.
    // @param callback  the callback function when send succeed or failed.  NULL means no callback.
    void send_response(
            const ReadBufferPtr& message, 
            const SendResponseCallback& callback)
    {
        async_send_message(message, callback);
    }

private:
    virtual void on_closed()
    {
        if (_closed_stream_callback)
        {
            _closed_stream_callback(
                    hdcs::networking::dynamic_pointer_cast<ServerStream>(shared_from_this()));
        }
    }

    // para1: response_message
    // para2: callback
    virtual bool on_sending(
            const ReadBufferPtr& ,
            const SendResponseCallback& )
    {
        return true;
    }

    // para1: response message
    virtual void on_sent(
            const ReadBufferPtr& ,
            const SendResponseCallback& callback)
    {
        if (callback) callback(HDCS_NETWORK_SUCCESS);
    }

    // para2 : response message.
    virtual void on_send_failed(
            ErrorCode error_code,
            const ReadBufferPtr& ,
            const SendResponseCallback& callback)
    {
        if (callback) callback(error_code);
    }

    virtual void on_received(
            const RequestPtr& request)
    {
        if (_received_request_callback)
        {
            _received_request_callback(
                    hdcs::networking::dynamic_pointer_cast<ServerStream>(shared_from_this()),
                    request);
        }
    }

private:
    ReceivedRequestCallback _received_request_callback;
    ClosedServerStreamCallback _closed_stream_callback;
    AtomicCounter _pending_process_count;
};

}
}

#endif
