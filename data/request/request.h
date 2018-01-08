#ifndef REQUEST_H_
#define REQUEST_H_

#include <memory>
#include <string>

#include <google/protobuf/message.h>

#include "../../common/smart_ptr/networking_ptr.h"
#include "../../buffer/buffer.h"
#include "../../common/ptime.h"
#include "../../stream/endpoint.h"
#include "../../controller/controller_impl.h"
#include "../../controller/controller.h"
#include "../../service_pool/service_pool.h"

namespace hdcs {
namespace networking {

class Request;
typedef hdcs::networking::shared_ptr<Request> RequestPtr;

class Request : public hdcs::networking::enable_shared_from_this<Request>
{
public:
    enum RequestType
    {
        BINARY,
        HTTP
    };

public:
    Request() {}
    virtual ~Request() {}

    // The request type: BINARY or HTTP.
    //virtual RequestType RequestType() = 0;

    // The method full name to request.
    virtual std::string Method() = 0;

    // The sequence id if supported.
    virtual uint64_t SequenceId() = 0;

    // Process the request.
    virtual void ProcessRequest(
            const ServerStreamWPtr& server_stream,
            const ServicePoolPtr& service_pool) = 0;

    // Assemble succeed response to buffer.
    virtual ReadBufferPtr AssembleSucceedResponse(
            const ControllerImplPtr& cntl,
            const google::protobuf::Message* response,
            std::string& err) = 0;

    // Assemble failed response to buffer.
    virtual ReadBufferPtr AssembleFailedResponse(
            int32_t error_code,
            const std::string& reason,
            std::string& err) = 0;

public:
    void CallMethod(
            MethodBoard* method_board,
            Controller* controller,
            google::protobuf::Message* request,
            google::protobuf::Message* response);

    void OnCallMethodDone(
            MethodBoard* method_board,
            Controller* controller,
            google::protobuf::Message* request,
            google::protobuf::Message* response);

    void SendSucceedResponse(
            const ControllerImplPtr& cntl,
            const google::protobuf::Message* response);

    void SendSucceedResponse(
            const ServerStreamWPtr& stream,
            const ReadBufferPtr& buffer);

    void SendFailedResponse(
            const ServerStreamWPtr& stream,
            int32_t error_code,
            const std::string& reason);

    void OnSendResponseDone(
            ErrorCode error_code);

    // Parse service & method name from method full name.
    // @return false if parse failed.
    static bool ParseMethodFullName(
            const std::string& method_full_name,
            std::string* service_name,
            std::string* method_name);

    // Find method board from service pool.
    // @return NULL if not found.
    static MethodBoard* FindMethodBoard(
            const ServicePoolPtr& service_pool,
            const std::string& service_name,
            const std::string& method_name);

    void SetLocalEndpoint(const Endpoint& local_endpoint)
    {
        _local_endpoint = local_endpoint;
    }

    const Endpoint& LocalEndpoint()
    {
        return _local_endpoint;
    }

    void SetRemoteEndpoint(const Endpoint& remote_endpoint)
    {
        _remote_endpoint = remote_endpoint;
    }

    const Endpoint& RemoteEndpoint()
    {
        return _remote_endpoint;
    }

    void SetReceivedTime(const PTime& received_time)
    {
        _received_time = received_time;
    }

    const PTime& ReceivedTime()
    {
        return _received_time;
    }

protected:
    Endpoint _local_endpoint;
    Endpoint _remote_endpoint;
    PTime _received_time;
};
/*
class BinaryRequest : public Request
{
public:
    BinaryRequest();
    virtual ~BinaryRequest();

    // why ???!!!!dehao
    //virtual RequestType RequestType();
    //virtual RequestType RequestType();
    RequestType xxx;

    virtual std::string Method();

    virtual uint64_t SequenceId();

    virtual void ProcessRequest(
            const ServerStreamWPtr& server_stream,
            const ServicePoolPtr& service_pool);

    virtual ReadBufferPtr AssembleSucceedResponse(
            const ControllerImplPtr& cntl,
            const google::protobuf::Message* response,
            std::string& err);

    virtual ReadBufferPtr AssembleFailedResponse(
            int32_t error_code,
            const std::string& reason,
            std::string& err);

private:
    friend class BinaryRequestParser;

 //   MessageHeader _req_header;
 //   Meta          _req_meta;
 //   ReadBufferPtr    _req_body;
};
*/

} 
}  

#endif
