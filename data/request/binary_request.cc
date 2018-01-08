#include "binary_request.h"
#include "../../common/error_code.h"

namespace hdcs {
namespace networking {

BinaryRequest::BinaryRequest() : _req_body(new ReadBuffer())
{
}

BinaryRequest::~BinaryRequest()
{
}

//Request::RequestType BinaryRequest::RequestType()
//{
//    return Request::BINARY;
//}

std::string BinaryRequest::Method()
{
    return _req_meta.method();
}

uint64_t BinaryRequest::SequenceId()
{
    return _req_meta.sequence_id();
}

void BinaryRequest::ProcessRequest(
        const ServerStreamWPtr& stream,
        const ServicePoolPtr& service_pool)
{
    std::string service_name;
    std::string method_name;
    if (!ParseMethodFullName(_req_meta.method(), &service_name, &method_name))
    {
        //LOG(ERROR) << "ProcessRequest(): " << EndpointToString(_remote_endpoint)
        //           << ": {" << _req_meta.sequence_id() << "}"
        //           << ": invalid method full name: " << _req_meta.method();
        SendFailedResponse(stream,
                HDCS_NETWORK_ERROR_PARSE_METHOD_NAME, "method full name: " + _req_meta.method());
        return;
    }

    MethodBoard* method_board = FindMethodBoard(service_pool, service_name, method_name);
    if (method_board == NULL)
    {
        //LOG(ERROR) << "ProcessRequest(): " << EndpointToString(_remote_endpoint)
        //           << ": {" << _req_meta.sequence_id() << "}"
        //           << ": method not found: " << _req_meta.method();
        SendFailedResponse(stream,
                HDCS_NETWORK_ERROR_FOUND_METHOD, "method full name: " + _req_meta.method());
        return;
    }

    google::protobuf::Service* service = method_board->GetServiceBoard()->Service();
    const google::protobuf::MethodDescriptor* method_desc = method_board->Descriptor();

    google::protobuf::Message* request = service->GetRequestPrototype(method_desc).New();
    CompressType compress_type =
        _req_meta.has_compress_type() ? _req_meta.compress_type(): CompressTypeNone;
    bool parse_request_return = false;
    if (compress_type == CompressTypeNone)
    {
        //parse_request_return = request->ParseFromZeroCopyStream(_req_body.get());
    }
    else
    {
        //hdcs::networking::scoped_ptr<AbstractCompressedInputStream> is(
        //        get_compressed_input_stream(_req_body.get(), compress_type));
        //parse_request_return = request->ParseFromZeroCopyStream(is.get());
    }
    if (!parse_request_return)
    {
        //LOG(ERROR) << "ProcessRequest(): " << EndpointToString(_remote_endpoint)
        //           << ": {" << _req_meta.sequence_id() << "}: parse request message failed";
        SendFailedResponse(stream,
                HDCS_NETWORK_ERROR_PARSE_REQUEST_MESSAGE, "method full name: " + _req_meta.method());
        delete request;
        return;
    }

    google::protobuf::Message* response = service->GetResponsePrototype(method_desc).New();

    Controller* controller = new Controller(); //  TODO
    const ControllerImplPtr& cntl = controller->impl();
    cntl->SetSequenceId(_req_meta.sequence_id());
    cntl->SetMethodId(_req_meta.method());
    cntl->SetLocalEndpoint(_local_endpoint);
    cntl->SetRemoteEndpoint(_remote_endpoint);
    cntl->SetServerStream(stream);
    if (_req_meta.has_server_timeout() && _req_meta.server_timeout() > 0)
    {
        cntl->SetServerTimeout(_req_meta.server_timeout());
    }
    cntl->SetRequestReceivedTime(_received_time);
    //cntl->SetResponseCompressType(_req_meta.has_expected_response_compress_type() ?
    //        _req_meta.expected_response_compress_type() : CompressTypeNone);

    CallMethod(method_board, controller, request, response);
}

ReadBufferPtr BinaryRequest::AssembleSucceedResponse(
        const ControllerImplPtr& cntl,
        const google::protobuf::Message* response,
        std::string& err)
{
    Meta meta;
    meta.set_type(Meta::RESPONSE);
    meta.set_sequence_id(cntl->SequenceId());
    meta.set_failed(false);
    //meta.set_compress_type(cntl->ResponseCompressType());

    MessageHeader header;
    int header_size = static_cast<int>(sizeof(header));
    WriteBuffer write_buffer;
    int64_t header_pos = write_buffer.Reserve(header_size);
    if (header_pos < 0)
    {
        err = "reserve message header failed";
        return ReadBufferPtr();
    }
    /*
    if (!meta.SerializeToZeroCopyStream(&write_buffer))
    {
        err = "serialize meta failed";
        return ReadBufferPtr();
    }
    */
    header.meta_size = static_cast<int>(write_buffer.ByteCount() - header_pos - header_size);
    bool ser_ret = false;
    /*
    if (meta.compress_type() == CompressTypeNone)
    {
        ser_ret = response->SerializeToZeroCopyStream(&write_buffer);
    }
    else
    {
        hdcs::networking::scoped_ptr<AbstractCompressedOutputStream> os(
                get_compressed_output_stream(&write_buffer, meta.compress_type()));
        ser_ret = response->SerializeToZeroCopyStream(os.get());
        os->Flush();
    }
    */
    if (!ser_ret)
    {
        err = "serialize response message failed";
        return ReadBufferPtr();
    }
    header.data_size = write_buffer.ByteCount() - header_pos - header_size - header.meta_size;
    header.message_size = header.meta_size + header.data_size;
    write_buffer.SetData(header_pos, reinterpret_cast<const char*>(&header), header_size);

    ReadBufferPtr read_buffer(new ReadBuffer());
    write_buffer.SwapOut(read_buffer.get());


    return read_buffer;
}

ReadBufferPtr BinaryRequest::AssembleFailedResponse(
        int32_t error_code,
        const std::string& reason,
        std::string& err)
{

    Meta meta;
    meta.set_type(Meta::RESPONSE);
    meta.set_sequence_id(_req_meta.sequence_id());
    meta.set_failed(true);
    meta.set_error_code(error_code);
    meta.set_reason(reason);

    MessageHeader header;
    int header_size = static_cast<int>(sizeof(header));
    WriteBuffer write_buffer;
    int64_t header_pos = write_buffer.Reserve(header_size);
    if (header_pos < 0)
    {
        err = "reserve rpc message header failed";
        return ReadBufferPtr();
    }
    /*
    if (!meta.SerializeToZeroCopyStream(&write_buffer))
    {
        err = "serialize meta failed";
        return ReadBufferPtr();
    }
    */
    header.meta_size = static_cast<int>(write_buffer.ByteCount() - header_pos - header_size);
    header.data_size = 0;
    header.message_size = header.meta_size + header.data_size;
    write_buffer.SetData(header_pos, reinterpret_cast<const char*>(&header), header_size);

    ReadBufferPtr read_buffer(new ReadBuffer());
    write_buffer.SwapOut(read_buffer.get());

    return read_buffer;
}

} // 
} // 
