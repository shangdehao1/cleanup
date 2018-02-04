#ifndef BINARY_REQUEST_H_
#define BINARY_REQUEST_H_

#include "request.h"
#include "../message/message_header.h"
#include "../message/meta.pb.h"

namespace hdcs {
namespace networking {

class BinaryRequestParser;

class BinaryRequest;
typedef hdcs::networking::shared_ptr<BinaryRequest> BinaryRequestPtr;

class BinaryRequest : public Request
{
public:
    BinaryRequest();
    virtual ~BinaryRequest();

    //virtual RequestType RequestType();

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

    MessageHeader _req_header; // 
    Meta          _req_meta;   //
    ReadBufferPtr    _req_body;//
};

}
}
#endif
