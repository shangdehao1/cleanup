#ifndef CLIENT_STREAM_H_
#define CLIENT_STREAM_H_

#include <functional>
#include <memory>
#include <map>

#include "../../common/error_code.h"
#include "../../common/smart_ptr/networking_ptr.h"
#include "client_message_stream.h"
#include "../../controller/controller_impl.h"
#include "../../data/message/meta.pb.h"

namespace hdcs {
namespace networking {

// Callback function when closed client stream.
typedef std::function<void(
        const ClientStreamPtr& )> ClosedClientStreamCallback;

class ClientStream : public MessageStream<ControllerImplPtr>
{
public:
    ClientStream(IOService& io_service, const Endpoint& endpoint)
        : MessageStream<ControllerImplPtr>(ROLE_TYPE_CLIENT, io_service, endpoint)
    {}

    virtual ~ClientStream() 
    {
        //FUNCTION_TRACE;
        for (ControllerMap::iterator it = _controller_map.begin();
                it != _controller_map.end(); ++it)
        {
            it->second->Done(HDCS_NETWORK_ERROR_CONNECTION_CLOSED, _error_message);
        }
    }

    // Set the callback function when closed stream.
    void set_closed_stream_callback(
            const ClosedClientStreamCallback& callback)
    {
        _closed_stream_callback = callback;
    }

    // Get the callback function when closed stream.
    const ClosedClientStreamCallback& closed_stream_callback() const
    {
        return _closed_stream_callback;
    }

    // Get pending process count.
    uint32_t pending_process_count()
    {
        ScopedLocker<FastLock> _(_controller_map_lock);
        return _controller_map.size(); 
    }

    // this interface will be called by channel.
    void call_method(const ControllerImplPtr& cntl) 
    {
        //FUNCTION_TRACE;

        if (is_closed())
        {
            //SLOG(ERROR, "call_method(): %s: stream already closed: %s",
            //        EndpointToString(_remote_endpoint).c_str(), _error_message);
            cntl->Done(HDCS_NETWORK_ERROR_CONNECTION_CLOSED, _error_message);
            return;
        }

        // add to map
        uint64_t sequence_id = cntl->SequenceId();
        {
            ScopedLocker<FastLock> _(_controller_map_lock);
            _controller_map[sequence_id] = cntl;
        }

        /// !!!Attention: the call may be already done at this point.
        async_send_message(cntl->RequestBuffer(), cntl);
    }

    // erase request from controller map by sequence id.
    void erase_request(uint64_t sequence_id)
    {
        ScopedLocker<FastLock> _(_controller_map_lock);
        _controller_map.erase(sequence_id);
    }

private:

    virtual void on_closed()
    {
        //FUNCTION_TRACE;

        ControllerMap tmp_map;
        {
            ScopedLocker<FastLock> _(_controller_map_lock);
            tmp_map.swap(_controller_map);
        }

        for (ControllerMap::iterator it = tmp_map.begin();
                it != tmp_map.end(); ++it)
        {
            it->second->Done(HDCS_NETWORK_ERROR_CONNECTION_CLOSED, _error_message);
        }

        if (_closed_stream_callback)
        {
            // TODO
            //_closed_stream_callback(
            //        hdcs::networking::dynamic_pointer_cast<ClientStream>(shared_from_this()));
        }
    }

    virtual bool on_sending(
            const ReadBufferPtr& , // request_message
            const ControllerImplPtr& cntl)
    {
        //FUNCTION_TRACE;

        // TODO: set rpc_meta.server_timeout here to make it more accurate.

        // if already done (may be done by timeout manager),
        // should cancel sending.
        return !cntl->IsDone() && !cntl->IsStartCancel();
    }

    virtual void on_sent(
            const ReadBufferPtr& request_message,
            const ControllerImplPtr& cntl)
    {
        //FUNCTION_TRACE;

        // the request has been sent.
        cntl->NotifyRequestSent(_local_endpoint, request_message->TotalCount());
    }

    virtual void on_send_failed(
            ErrorCode error_code,
            const ReadBufferPtr& , // 
            const ControllerImplPtr& cntl)
    {
        //FUNCTION_TRACE;

        // TODO more efficient sync map
        // remove the call handle from map
        uint64_t sequence_id = cntl->SequenceId();
        {
            ScopedLocker<FastLock> _(_controller_map_lock);
            _controller_map.erase(sequence_id);
        }

        cntl->Done(error_code, _error_message);
    }

    virtual void on_received(
            const ReadBufferPtr& message,
            int meta_size,
            int64_t data_size)
    {
        //FUNCTION_TRACE;
        // TODO TODO TODO TODO TODO TODO
        Meta meta;
        //if (!meta.ParseFromBoundedZeroCopyStream(message.get(), meta_size))
        if(false)
        {
            //SLOG(ERROR, "on_received(): %s: parse rpc meta failed, ignore",
            //        EndpointToString(_remote_endpoint).c_str());
            return;
        }

        if (meta.type() != Meta::RESPONSE)
        {
            // TODO handle un-expected message type
            //
            // just ignore it
            //SLOG(ERROR, "on_received(): %s {%lu}: un-expected message type %d, ignore",
            //        EndpointToString(_remote_endpoint).c_str(),
            //        meta.sequence_id(), meta.type());
            return;
        }

        // find corresponding call handle and erase from map
        // TODO more efficient sync map
        ControllerImplPtr cntl;
        {
            ScopedLocker<FastLock> _(_controller_map_lock);
            ControllerMap::iterator it = _controller_map.find(meta.sequence_id());
            if (it != _controller_map.end())
            {
                cntl = it->second;
                _controller_map.erase(it);
            }
        }

        if (!cntl)
        {
            // TODO handle un-expected sequence id
            //
            // just ignore it
            //SLOG(WARNING, "on_received(): %s {%lu}: sequence id not found (maybe already timeout), ignore",
            //        EndpointToString(_remote_endpoint).c_str(), meta.sequence_id());
            return;
        }

        if (cntl->IsDone())
        {
            // just ignore it
            //SLOG(WARNING, "on_received(): %s {%lu}: request already done (maybe already timeout), ignore",
            //        EndpointToString(_remote_endpoint).c_str(), meta.sequence_id());
            return;
        }

        if (!meta.has_failed())
        {
            //SLOG(ERROR,  "on_received(): %s {%lu}: bad rpc meta: \"failed\" field not set",
            //        EndpointToString(_remote_endpoint).c_str(), meta.sequence_id());
            cntl->Done(HDCS_NETWORK_ERROR_PARSE_RESPONSE_MESSAGE, "meta: \"failed\" field not set");
        }
        else if (meta.failed())
        {
            if (!meta.has_error_code())
            {
                //SLOG(ERROR,  "on_received(): %s {%lu}: bad rpc meta: \"error_code\" field not set",
                //        EndpointToString(_remote_endpoint).c_str(), meta.sequence_id());
                cntl->Done(HDCS_NETWORK_ERROR_PARSE_RESPONSE_MESSAGE, "meta: \"error_code\" field not set");
            }
            else if (!meta.has_reason())
            {
                //SLOG(ERROR,  "on_received(): %s {%lu}: bad rpc meta: \"reason\" field not set",
                //        EndpointToString(_remote_endpoint).c_str(), meta.sequence_id());
                cntl->Done(HDCS_NETWORK_ERROR_PARSE_RESPONSE_MESSAGE, "meta: \"reason\" field not set");
            }
            else
            {
                cntl->Done(meta.error_code(), meta.reason());
            }
        }
        else // !meta.failed()
        {
            //SCHECK_EQ(data_size, message->TotalCount() - message->ByteCount());
            cntl->SetResponseBuffer(message);
            //cntl->SetResponseCompressType(meta.has_compress_type() ?
            //        meta.compress_type() : CompressTypeNone);
            cntl->Done(HDCS_NETWORK_SUCCESS, "succeed");
        }
    }

private:
    ClosedClientStreamCallback _closed_stream_callback;

    // sequence_id ==> ControllerImplPtr
    // TODO more efficient sync map
    typedef std::map<uint64_t, ControllerImplPtr> ControllerMap; // !!!Attention
    ControllerMap _controller_map;
    FastLock _controller_map_lock;
    //DISALLOW_EVIL_CONSTRUCTORS(ClientStream);
};

}
}

#endif
