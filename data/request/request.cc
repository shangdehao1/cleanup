
#include "request.h"
#include "../../stream/server/server_stream.h"
#include "../../closure/closure.h"

namespace hdcs {
namespace networking{

void Request::CallMethod(
        MethodBoard* method_board,
        Controller* controller,
        google::protobuf::Message* request,
        google::protobuf::Message* response)
{

    PTime time_now = ptime_now();
    const ControllerImplPtr& cntl = controller->impl();

    ServerStreamPtr stream = cntl->ServerStream().lock();
    if (!stream)
    {
        //SLOG(WARNING, "CallMethod(): %s {%lu}: stream already closed, not process request",
        //        EndpointToString(_remote_endpoint).c_str(), cntl->SequenceId());
        delete request;
        delete response;
        delete controller;
        return;
    }

    if (cntl->ServerTimeout() > 0)
    {
        int64_t server_wait_time_us =
            (time_now - cntl->RequestReceivedTime()).total_microseconds();
        if (server_wait_time_us > cntl->ServerTimeout() * 1000)
        {
        //SLOG(WARNING, "CallMethod(): %s {%lu}: wait processing timeout, not process request: "
        //       "server_timeout_us=%lld, server_wait_time_us=%lld",
        //        EndpointToString(_remote_endpoint).c_str(), cntl->SequenceId(),
        //        (cntl->ServerTimeout() * 1000), server_wait_time_us);
            delete request;
            delete response;
            delete controller;
            return;
        }
    }

    google::protobuf::Closure* done = NewClosure(
            shared_from_this(), &Request::OnCallMethodDone, // callback: OnCallMthodDone
            method_board, controller, request, response);
    cntl->SetStartProcessTime(time_now);
    stream->increase_pending_process_count();
    method_board->ReportProcessBegin();
    method_board->GetServiceBoard()->Service()->CallMethod(
            method_board->Descriptor(), controller, request, response, done);
}

void Request::OnCallMethodDone(
        MethodBoard* method_board,
        Controller* controller,
        google::protobuf::Message* request,
        google::protobuf::Message* response)
{
    PTime time_now = ptime_now();
    const ControllerImplPtr& cntl = controller->impl();
    cntl->SetFinishProcessTime(time_now);
    int64_t process_time_us =
        (cntl->FinishProcessTime() - cntl->StartProcessTime()).total_microseconds();
    method_board->ReportProcessEnd(!cntl->Failed(), process_time_us);

    ServerStreamPtr stream = cntl->ServerStream().lock();
    if (!stream)
    {
        //SLOG(WARNING, "OnCallMethodDone(): %s {%lu}: stream already closed, not response",
        //        EndpointToString(_remote_endpoint).c_str(), cntl->SequenceId());
        delete request;
        delete response;
        delete controller;
        return;
    }
    stream->decrease_pending_process_count();

    if (cntl->ServerTimeout() > 0)
    {
        int64_t server_used_time_us =
            (cntl->FinishProcessTime() - cntl->RequestReceivedTime()).total_microseconds();
        if (server_used_time_us > cntl->ServerTimeout() * 1000)
        {
            //SLOG(WARNING, "OnCallMethodDone(): %s {%lu}: call method \"%s\" timeout, not response: "
            //    "server_timeout_us=%lld, server_used_time_us=%lld, process_time_us=%lld",
            //    EndpointToString(_remote_endpoint).c_str(), cntl->SequenceId(),
            //    cntl->MethodId().c_str(), (cntl->ServerTimeout() * 1000),
            //    server_used_time_us, process_time_us);
            delete request;
            delete response;
            delete controller;
            return;
        }
    }

    if (cntl->Failed())
    {
        //SLOG(ERROR, "OnCallMethodDone(): %s {%lu}: call method \"%s\" failed: %s: %s",
        //        EndpointToString(_remote_endpoint).c_str(), cntl->SequenceId(),
        //        cntl->MethodId().c_str(),
        //        ErrorCodeToString(cntl->ErrorCode()), cntl->Reason().c_str());
        SendFailedResponse(cntl->ServerStream(), cntl->ErrorCode(), cntl->Reason());
    }
    else
    {
        //SLOG(DEBUG, "OnCallMethodDone(): %s {%lu}: call method \"%s\" succeed",
        //        EndpointToString(_remote_endpoint).c_str(), cntl->SequenceId(),
        //        cntl->MethodId().c_str());
        SendSucceedResponse(cntl, response);
    }

    delete request;
    delete response;
    delete controller;
}

void Request::SendSucceedResponse(
        const ControllerImplPtr& cntl,
        const google::protobuf::Message* response)
{
    std::string err;
    ReadBufferPtr read_buffer = AssembleSucceedResponse(cntl, response, err);
    if (!read_buffer)
    {
        //SLOG(ERROR, "SendSucceedResponse(): %s {%lu}: assemble response buffer failed: %s",
        //        EndpointToString(_remote_endpoint).c_str(), SequenceId(), err.c_str());
        return;
    }

    SendSucceedResponse(cntl->ServerStream(), read_buffer);
}

void Request::SendSucceedResponse(
        const ServerStreamWPtr& stream,
        const ReadBufferPtr& buffer)
{
    ServerStreamPtr real_stream = stream.lock();
    if (!real_stream)
    {
        //SLOG(ERROR, "SendSucceedResponse(): %s {%lu}: stream already closed, ignore",
        //        EndpointToString(_remote_endpoint).c_str(), SequenceId());
        return;
    }

    real_stream->send_response(buffer,
            boost::bind(&Request::OnSendResponseDone, shared_from_this(), _1));
}

void Request::SendFailedResponse(
        const ServerStreamWPtr& stream,
        int32_t error_code,
        const std::string& reason)
{
    uint64_t sequence_id = SequenceId();
    ServerStreamPtr real_stream = stream.lock();
    if (!real_stream)
    {
        //SLOG(ERROR, "SendFailedResponse(): %s {%lu}: stream already closed, ignore",
        //        EndpointToString(_remote_endpoint).c_str(), sequence_id);
        return;
    }

    std::string err;
    // virtual method: AssembleFailedResponse
    ReadBufferPtr read_buffer = AssembleFailedResponse(error_code, reason, err);
    if (!read_buffer)
    {
        //SLOG(ERROR, "SendFailedResponse(): %s {%lu}: assemble response buffer failed: %s",
        //        EndpointToString(_remote_endpoint).c_str(), sequence_id, err.c_str());
        return;
    }

    real_stream->send_response(read_buffer,
            boost::bind(&Request::OnSendResponseDone, shared_from_this(), _1));
}

void Request::OnSendResponseDone(
        ErrorCode error_code)
{
    if (error_code == HDCS_NETWORK_SUCCESS)
    {
        //SLOG(DEBUG, "OnSendResponseDone(): %s {%lu}: send succeed",
        //        EndpointToString(_remote_endpoint).c_str(), SequenceId());
    }
    else
    {
        //SLOG(ERROR, "OnSendResponseDone(): %s {%lu}: send failed: %s",
        //        EndpointToString(_remote_endpoint).c_str(), SequenceId(),
        //        ErrorCodeToString(error_code));
    }
}

bool Request::ParseMethodFullName(
        const std::string& method_full_name,
        std::string* service_name,
        std::string* method_name)
{
    std::string::size_type pos = method_full_name.rfind('.');
    if (pos == std::string::npos)
    {
        return false;
    }
    *service_name = method_full_name.substr(0, pos);
    *method_name = method_full_name.substr(pos + 1);
    return true;
}

MethodBoard* Request::FindMethodBoard(
        const ServicePoolPtr& service_pool,
        const std::string& service_name,
        const std::string& method_name)
{
    ServiceBoard* service_board = service_pool->FindService(service_name);
    if (service_board == NULL)
    {
        return NULL;
    }
    const google::protobuf::MethodDescriptor* method_desc =
        service_board->Descriptor()->FindMethodByName(method_name);
    if (method_desc == NULL)
    {
        return NULL;
    }
    return service_board->Method(method_desc->index());
}

} 
} 
