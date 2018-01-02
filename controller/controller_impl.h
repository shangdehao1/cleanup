#ifndef CONTROLLER_IMPL_H_
#define CONTROLLER_IMPL_H_

#include <deque>
#include <list>
#include <string>
#include <functional>
#include <memory>

#include "../stream/endpoint.h"
#include "../common/ptime.h"
#include "../common/error_code.h"
#include "../common/atomic.h"
#include "../common/wait_event.h"
#include "../common/smart_ptr/networking_ptr.h"
#include "../buffer/buffer.h"

//#include <google/protobuf/descriptor.h>


namespace hdcs {
namespace networking {

#define CompressTypeAuto ((CompressType)-1)

class ControllerImpl;

typedef hdcs::networking::shared_ptr<ControllerImpl> ControllerImplPtr;

class ControllerImpl : public hdcs::networking::enable_shared_from_this<ControllerImpl>
{
public:
//    typedef hdcs::networking::shared_ptr<ControllerImpl> ControllerImplPtr;
    typedef hdcs::networking::shared_ptr<hdcs::networking::WaitEvent> WaitEventPtr;
    typedef std::function<void(const ControllerImplPtr&)> InternalDoneCallback;
    

public:

    ControllerImpl()
        : _sequence_id(0)
        , _error_code(HDCS_NETWORK_SUCCESS)
        , _is_done(NOT_DONE)
        , _is_request_sent(false)
        , _sent_bytes(0)
        , _is_start_cancel(false)
        , _is_sync(false)
        , _timeout_id(0)
        , _server_timeout(0)
        , _is_http(false)
        , _http_path(NULL)
        , _http_query_params(NULL)
        , _http_headers(NULL)
    {}

    virtual ~ControllerImpl() {}

    //  some operation for RequestOptions. TODO
    /*
    void SetRequestCompressType(CompressType compress_type)
    {
        _user_options.request_compress_type = compress_type;
    }

    CompressType RequestCompressType()
    {
        return _user_options.request_compress_type != CompressTypeAuto ?
            _user_options.request_compress_type : _auto_options.request_compress_type;
    }

    void SetResponseCompressType(CompressType compress_type)
    {
        _user_options.response_compress_type = compress_type;
    }

    CompressType ResponseCompressType()
    {
        return _user_options.response_compress_type != CompressTypeAuto ?
            _user_options.response_compress_type : _auto_options.response_compress_type;
    }
    */

    void SetTimeout(int64_t timeout)
    {
        _user_options.timeout = timeout;
    }

    int64_t Timeout() const
    {
        return _user_options.timeout > 0 ?
            _user_options.timeout : _auto_options.timeout;
    }

    // return some data member value 
    bool Failed() const
    {
        return _error_code != HDCS_NETWORK_SUCCESS;
    }

    int ErrorCode() const
    {
        return _error_code;
    }

    const std::string& Reason() const
    {
        return _reason;
    }

    std::string ErrorText() const
    {
        if (_reason.empty()) {
            return ErrorCodeToString(_error_code);
        } else {
            return ErrorCodeToString(_error_code)
                + std::string(": ") + _reason;
        }
    }

    // TODO
    void StartCancel()
    {
        // TODO to support
    }

    void SetFailed(const std::string& reason)
    {
        SetFailed(HDCS_NETWORK_ERROR_FROM_USER, reason);
    }

    bool IsCanceled() const
    {
        // TODO to support
        return false;
    }

    //void NotifyOnCancel(google::protobuf::Closure*  callback )
    //{
    //    TODO to support
    //}
     void NotifyOnCancel( )
     {
          //TODO to support
     }


    // -----------------------------------------------------------------
    // set/read some information 
    // Used both in client and server.
    // -----------------------------------------------------------------
    void SetSequenceId(uint64_t sequence_id)
    {
        _sequence_id = sequence_id;
    }

    uint64_t SequenceId() const
    {
        return _sequence_id;
    }

    void SetMethodId(const std::string& method_id)
    {
        _method_id = method_id;
    }

    const std::string& MethodId() const
    {
        return _method_id;
    }

    void SetLocalEndpoint(const Endpoint& local_endpoint)
    {
        _local_endpoint = local_endpoint;
    }

    const Endpoint& LocalEndpoint() const
    {
        return _local_endpoint;
    }

    void SetRemoteEndpoint(const Endpoint& remote_endpoint)
    {
        _remote_endpoint = remote_endpoint;
    }

    const Endpoint& RemoteEndpoint() const
    {
        return _remote_endpoint;
    }

    void SetFailed(int error_code, const std::string& reason)
    {
        _error_code = error_code;
        _reason = reason;
        if (_error_code == HDCS_NETWORK_ERROR_REQUEST_TIMEOUT)
        {
            _reason += _is_request_sent ?
                ": request already sent to remote" :
                ": request not sent to remote";
        }
    }


    // -----------------------------------------------------------------
    // Used only in client.
    // -----------------------------------------------------------------
    bool IsDone() const
    {
        return _is_done == DONE;
    }

    bool IsRequestSent() const
    {
        return _is_request_sent;
    }

    int64_t SentBytes() const
    {
        return _sent_bytes;
    }

    bool IsStartCancel() const
    {
        return _is_start_cancel;
    }

    void PushDoneCallback(const InternalDoneCallback& callback)
    {
        //SCHECK(callback);
        _done_callbacks.push_back(callback);
    }

    void Done(int error_code, const std::string& reason)
    {
        // make sure that the callback is only called once.
        if (atomic_comp_swap(&_is_done, DONE, NOT_DONE) == NOT_DONE)
        {
            SetFailed(error_code, reason);

            while (!_done_callbacks.empty())
            {
                const InternalDoneCallback& callback = _done_callbacks.back();
                callback(shared_from_this());
                _done_callbacks.pop_back();
            }
        }
    }
    /*
    void FillFromMethodDescriptor(const google::protobuf::MethodDescriptor* method)
    {
        _method_id = method->full_name();
        if (_user_options.timeout <= 0)
        {
            int64 timeout_in_ms = method->options().HasExtension(method_timeout) ?
                method->options().GetExtension(method_timeout) :
                method->service()->options().GetExtension(service_timeout);
            if (timeout_in_ms <= 0) {
                // Just a protection, it shouldn't happen.
                timeout_in_ms = 1;
            }
            _auto_options.timeout = timeout_in_ms;
        }
        if (_user_options.request_compress_type == CompressTypeAuto) {
            _auto_options.request_compress_type =
                method->options().HasExtension(request_compress_type) ?
                method->options().GetExtension(request_compress_type) :
                CompressTypeNone;
        }
        if (_user_options.response_compress_type == CompressTypeAuto) {
            _auto_options.response_compress_type =
                method->options().HasExtension(response_compress_type) ?
                method->options().GetExtension(response_compress_type) :
                CompressTypeNone;
        }
    }
   */

    void SetSync()
    {
        _is_sync = true;
    }

    bool IsSync() const
    {
        return _is_sync;
    }

    void SetWaitEvent(const WaitEventPtr& wait_event)
    {
        _wait_event = wait_event;
    }

    const WaitEventPtr& WaitEvent() const
    {
        return _wait_event;
    }
/*   // TODO
    void SetClientStream(const ClientStreamWPtr& stream)
    {
        _client_stream = stream;
    }

    const ClientStreamWPtr& ClientStream() const
    {
        return _client_stream;
    }
*/
    void StartTimer()
    {
        int64_t timeout = Timeout();
        _expiration = timeout <= 0 ? ptime_infin()
            : ptime_now() + time_duration_milliseconds(timeout);
    }

    const PTime& Expiration() const
    {
        return _expiration;
    }

    void SetTimeoutId(uint64_t timeout_id)
    {
        _timeout_id = timeout_id;
    }

    uint64_t TimeoutId() const
    {
        return _timeout_id;
    }

    void SetRequestBuffer(const ReadBufferPtr& request_buffer)
    {
        _request_buffer = request_buffer;
    }

    const ReadBufferPtr& RequestBuffer() const
    {
        return _request_buffer;
    }

    void SetResponseBuffer(const ReadBufferPtr& response_buffer)
    {
        _response_buffer = response_buffer;
    }

    const ReadBufferPtr& ResponseBuffer() const
    {
        return _response_buffer;
    }

    void NotifyRequestSent(const Endpoint& local_endpoint, int64_t sent_bytes)
    {
        _is_request_sent = true;
        _local_endpoint = local_endpoint;
        _sent_bytes = sent_bytes;
        _request_sent_time = ptime_now();
    }


    // -----------------------------------------------------------------
    // Used only in server.
    // -----------------------------------------------------------------
    /* TODO
    void SetServerStream(const ServerStreamWPtr& stream)
    {
        _server_stream = stream;
    }

    const ServerStreamWPtr& ServerStream() const
    {
        return _server_stream;
    }
    */
    void SetServerTimeout(int64_t timeout)
    {
        _server_timeout = timeout;
    }

    int64_t ServerTimeout() const
    {
        return _server_timeout;
    }

    void SetRequestReceivedTime(const PTime& time)
    {
        _request_received_time = time;
    }

    const PTime& RequestReceivedTime()
    {
        return _request_received_time;
    }

    void SetStartProcessTime(const PTime& time)
    {
        _start_process_time = time;
    }

    const PTime& StartProcessTime()
    {
        return _start_process_time;
    }

    void SetFinishProcessTime(const PTime& time)
    {
        _finish_process_time = time;
    }

    const PTime& FinishProcessTime()
    {
        return _finish_process_time;
    }

    void SetHttp()
    {
        _is_http = true;
    }

    bool IsHttp() const
    {
        return _is_http;
    }

    void SetHttpPath(const std::string* path)
    {
        _http_path = path;
    }

    const std::string& HttpPath() const
    {
        //SCHECK(_is_http);
        return *_http_path;
    }

    void SetHttpQueryParams(const std::map<std::string, std::string>* params)
    {
        _http_query_params = params;
    }

    const std::map<std::string, std::string>& HttpQueryParams() const
    {
        //SCHECK(_is_http);
        return *_http_query_params;
    }

    void SetHttpHeaders(const std::map<std::string, std::string>* headers)
    {
        _http_headers = headers;
    }

    const std::map<std::string, std::string>& HttpHeaders() const
    {
        //SCHECK(_is_http);
        return *_http_headers;
    }

private:

    uint64_t _sequence_id;
    std::string _method_id;
    Endpoint _local_endpoint;
    Endpoint _remote_endpoint;
    int _error_code;
    std::string _reason;

    // used only in client side
    static const int NOT_DONE = 0;
    static const int DONE = 1;
    volatile int _is_done; // 0 means not done, 1 means aleady done
    bool _is_request_sent; // if the request has been sent
    int64_t _sent_bytes; // sent bytes including the header
    bool _is_start_cancel; // if has started canceling the call
    std::deque<InternalDoneCallback> _done_callbacks; // internal done callbacks
    bool _is_sync;
    WaitEventPtr _wait_event; // used only for sync call
    //ClientStreamWPtr _client_stream;
    PTime _expiration;
    uint64_t _timeout_id;
    ReadBufferPtr _request_buffer;
    ReadBufferPtr _response_buffer;
    PTime _request_sent_time;

    struct RequestOptions {
        int64_t timeout;
        //CompressType request_compress_type;
        //CompressType response_compress_type;
        RequestOptions() :
            timeout(0)
            //request_compress_type(CompressTypeAuto),
            //response_compress_type(CompressTypeAuto)
            {}
    };
    RequestOptions _user_options; // options set by user
    RequestOptions _auto_options; // options from proto

    // used only in server side
    //ServerStreamWPtr _server_stream;
    int64_t _server_timeout;
    PTime _request_received_time;
    PTime _start_process_time;
    PTime _finish_process_time;

    bool _is_http;
    const std::string* _http_path;
    const std::map<std::string, std::string>* _http_query_params;
    const std::map<std::string, std::string>* _http_headers;
};

} 
}

#endif
