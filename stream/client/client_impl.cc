#include <algorithm>
#include <deque>

#include "client_impl.h"

namespace hdcs {
namespace networking {

ClientImpl::ClientImpl(const ClientOptions& options)
    : _options(options)
    , _is_running(false)
    , _next_request_id(0)
    , _epoch_time(ptime_now())
    , _ticks_per_second(time_duration_seconds(1).ticks())
    , _last_maintain_ticks(0)
    , _last_print_connection_ticks(0)
{
    _slice_count = std::max(1, 1000 / MAINTAIN_INTERVAL_IN_MS);
    _slice_quota_in = _options.max_throughput_in == -1 ?
        -1 : std::max(0L, _options.max_throughput_in * 1024L * 1024L) / _slice_count;
    _slice_quota_out = _options.max_throughput_out == -1 ?
        -1 : std::max(0L, _options.max_throughput_out * 1024L * 1024L) / _slice_count;
    _max_pending_buffer_size =
        std::max(0L, _options.max_pending_buffer_size * 1024L * 1024L);
    _keep_alive_ticks = _options.keep_alive_time == -1 ?
        -1 : std::max(1, _options.keep_alive_time) * _ticks_per_second;
    
    //LOG(INFO) << "ClientImpl(): quota_in="
    //          << (_slice_quota_in == -1 ? -1 : _slice_quota_in * _slice_count / (1024L * 1024L))
    //          << "MB/s, quota_out="
    //          << (_slice_quota_out == -1 ? -1 : _slice_quota_out * _slice_count / (1024L * 1024L))
    //          << "MB/s, max_pending_buffer_size="
    //          << (_max_pending_buffer_size / (1024L * 1024L))
    //          << "MB, keep_alive_time="
    //          << (_keep_alive_ticks == -1 ? -1 : _keep_alive_ticks / _ticks_per_second)
    //          << "seconds";
}

ClientImpl::~ClientImpl()
{
    //FUNCTION_TRACE;
    Stop();
}

void ClientImpl::Start()
{
    /*
    ScopedLocker<MutexLock> _(_start_stop_lock);
    if (_is_running) return;

    _flow_controller.reset(new FlowController(
                _slice_quota_in == -1, _slice_quota_in, // no limits for every connection...sdh
                _slice_quota_out == -1, _slice_quota_out));

    _maintain_thread_group.reset(new ThreadGroupImpl(
                2, "client_maintain_thread_group"));
    _maintain_thread_group->start();

    _callback_thread_group.reset(new ThreadGroupImpl(
                _options.callback_thread_num, "sofa_pbrpc_client_callback_thread_group"));
    _callback_thread_group->start();

    _work_thread_group.reset(new ThreadGroupImpl(
                _options.work_thread_num, "sofa_pbrpc_client_work_thread_group"));
    _work_thread_group->start();

    _timer_worker.reset(new TimerWorker(_maintain_thread_group->io_service()));
    _timer_worker->set_time_duration(time_duration_milliseconds(MAINTAIN_INTERVAL_IN_MS)); 
    _timer_worker->set_work_routine(boost::bind(
                &ClientImpl::TimerMaintain, shared_from_this(), _1));
    _timer_worker->start();

    _timeout_manager.reset(new TimeoutManager(_maintain_thread_group->io_service()));
    _timeout_manager->start();

    _is_running = true;

    //LOG(INFO) << "Start(): rpc client started";
    */
}

void ClientImpl::Stop()
{
    /*
    ScopedLocker<MutexLock> _(_start_stop_lock);
    if (!_is_running) return;
    _is_running = false;

    _timeout_manager->stop();
    _timer_worker->stop();
    StopStreams();
    _work_thread_group->stop();

    _timeout_manager.reset();
    _timer_worker.reset();
    ClearStreams();
    _callback_thread_group->stop();
    _maintain_thread_group->stop();

    _work_thread_group.reset();
    _callback_thread_group.reset();
    _maintain_thread_group.reset();
    _flow_controller.reset();
    */

    //LOG(INFO) << "Stop(): rpc client stopped";
}

ClientOptions ClientImpl::GetOptions()
{
    return _options;
}

void ClientImpl::ResetOptions(const ClientOptions& options)
{
    /*
    int64_t old_slice_quota_in = _slice_quota_in;
    int64_t old_slice_quota_out = _slice_quota_out;
    int64_t old_max_pending_buffer_size = _max_pending_buffer_size;
    int64_t old_keep_alive_ticks = _keep_alive_ticks;

    _options.max_throughput_in = options.max_throughput_in;
    _options.max_throughput_out = options.max_throughput_out;
    _options.max_pending_buffer_size = options.max_pending_buffer_size;
    _options.keep_alive_time = options.keep_alive_time;

    _slice_quota_in = _options.max_throughput_in == -1 ?
        -1 : std::max(0L, _options.max_throughput_in * 1024L * 1024L) / _slice_count;
    _slice_quota_out = _options.max_throughput_out == -1 ?
        -1 : std::max(0L, _options.max_throughput_out * 1024L * 1024L) / _slice_count;
    _max_pending_buffer_size =
        std::max(0L, _options.max_pending_buffer_size * 1024L * 1024L);
    _keep_alive_ticks = _options.keep_alive_time == -1 ?
        -1 : std::max(1, _options.keep_alive_time) * _ticks_per_second;

    if (_max_pending_buffer_size != old_max_pending_buffer_size)
    {
        ScopedLocker<FastLock> _(_stream_map_lock);
        for (StreamMap::iterator it = _stream_map.begin(); it != _stream_map.end(); ++it)
        {
            it->second->set_max_pending_buffer_size(_max_pending_buffer_size);
        }
    }

    LOG(INFO) << "ResetOptions(): quota_in="
              << (_slice_quota_in == -1 ? -1 : _slice_quota_in * _slice_count / (1024L * 1024L))
              << "MB/s(old "
              << (old_slice_quota_in == -1 ? -1 : old_slice_quota_in * _slice_count / (1024L * 1024L))
              << "MB/s), quota_out="
              << (_slice_quota_out == -1 ? -1 : _slice_quota_out * _slice_count / (1024L * 1024L))
              << "MB/s(old "
              << (old_slice_quota_out == -1 ? -1 : old_slice_quota_out * _slice_count / (1024L * 1024L))
              << "MB/s), max_pending_buffer_size="
              << (_max_pending_buffer_size / (1024L * 1024L))
              << "MB(old "
              << (old_max_pending_buffer_size / (1024L * 1024L))
              << "MB), keep_alive_time="
              << (_keep_alive_ticks == -1 ? -1 : _keep_alive_ticks / _ticks_per_second)
              << "seconds(old "
              << (old_keep_alive_ticks == -1 ? -1 : old_keep_alive_ticks / _ticks_per_second)
              << "seconds)";
              */
}

int ClientImpl::ConnectionCount()
{
    ScopedLocker<FastLock> _(_stream_map_lock);
    return _stream_map.size();
}

//  call method to remote endpoint.
//
// The call can be done in following cases:
//
// * send failed
// * timeouted
// * received response
void ClientImpl::CallMethod(const google::protobuf::Message* request,
        google::protobuf::Message* response,
        const ControllerImplPtr& cntl)
{
    /*
    if (!_is_running)
    {
        SLOG(ERROR, "CallMethod(): client not in running, ignore");
        cntl->Done(RPC_ERROR_NOT_IN_RUNNING, "client not in running, should start it first");
        return;
    }

    // get the stream
    // --stream means the connection between client and server.--sdh 
    ClientStreamPtr stream = FindOrCreateStream(cntl->RemoteEndpoint());
    if (!stream)
    {
        SLOG(ERROR, "CallMethod(): create socket stream failed: %s",
                EndpointToString(cntl->RemoteEndpoint()).c_str());
        cntl->Done(RPC_ERROR_CREATE_STREAM, "create stream failed, maybe exceed connection limit");
        return;
    }
    cntl->SetClientStream(stream);

    // check the pending buffer full
    if (stream->pending_buffer_size() > stream->max_pending_buffer_size())
    {
        SLOG(DEBUG, "CallMethod(): pending buffer full: %s",
                EndpointToString(cntl->RemoteEndpoint()).c_str());
        cntl->Done(RPC_ERROR_SEND_BUFFER_FULL, "pending buffer full");
        return;
    }

    // generate sequence id
    cntl->SetSequenceId(GenerateSequenceId());

    // prepare request buffer
    Meta meta;
    meta.set_type(Meta::REQUEST);
    meta.set_sequence_id(cntl->SequenceId());
    meta.set_method(cntl->MethodId());
    int64_t timeout = cntl->Timeout();
    if (timeout > 0)
    {
        meta.set_server_timeout(timeout);
    }
    meta.set_compress_type(cntl->RequestCompressType());
    meta.set_expected_response_compress_type(cntl->ResponseCompressType());

    MessageHeader header;
    int header_size = sizeof(header);
    WriteBuffer write_buffer;
    int64_t header_pos = write_buffer.Reserve(header_size);
    if (header_pos < 0)
    {
        LOG(ERROR) << "CallMethod(): " << EndpointToString(cntl->RemoteEndpoint())
                   << ": reserve rpc message header failed";
        cntl->Done(RPC_ERROR_SERIALIZE_REQUEST, "reserve rpc message header failed");
        return;
    }
    if (!meta.SerializeToZeroCopyStream(&write_buffer))
    {
        LOG(ERROR) << "CallMethod(): " << EndpointToString(cntl->RemoteEndpoint())
                   << ": serialize rpc meta failed";
        cntl->Done(RPC_ERROR_SERIALIZE_REQUEST, "serialize rpc meta failed");
        return;
    }
    header.meta_size = static_cast<int>(write_buffer.ByteCount() - header_pos - header_size);
    bool serialize_request_return = false;
    if (meta.compress_type() == CompressTypeNone)
    {
        serialize_request_return = request->SerializeToZeroCopyStream(&write_buffer);
    }
    else
    {
        ::sofa::pbrpc::scoped_ptr<AbstractCompressedOutputStream> os(
                get_compressed_output_stream(&write_buffer, meta.compress_type()));
        serialize_request_return = request->SerializeToZeroCopyStream(os.get());
        os->Flush();
    }
    if (!serialize_request_return)
    {
        LOG(ERROR) << "CallMethod(): " << EndpointToString(cntl->RemoteEndpoint())
                   << ": serialize request message failed";
        cntl->Done(RPC_ERROR_SERIALIZE_REQUEST, "serialize request message failed");
        return;
    }
    header.data_size = write_buffer.ByteCount() - header_pos - header_size - header.meta_size;
    header.message_size = header.meta_size + header.data_size;
    write_buffer.SetData(header_pos, reinterpret_cast<const char*>(&header), header_size);

    ReadBufferPtr read_buffer(new ReadBuffer());
    write_buffer.SwapOut(read_buffer.get());
    cntl->SetRequestBuffer(read_buffer);

    // push callback
    cntl->PushDoneCallback(boost::bind(&ClientImpl::DoneCallback, shared_from_this(), response, _1));

    // add to timeout manager if need
    if (timeout > 0)
    {
        if (!_timeout_manager->add(cntl))
        {
            LOG(ERROR) << "CallMethod(): " << EndpointToString(cntl->RemoteEndpoint())
                       << ": add to timeout manager failed: timeout=" << timeout << "ms";
            cntl->Done(RPC_ERROR_REQUEST_TIMEOUT, "add to timeout manager failed, maybe too short timeout");
            return;
        }
    }

    // call method
    stream->call_method(cntl);
    */
}

const ThreadGroupImplPtr& ClientImpl::GetCallbackThreadGroup() const
{
    return _callback_thread_group;
}

bool ClientImpl::ResolveAddress(const std::string& address,
        Endpoint* endpoint)
{
    return hdcs::networking::ResolveAddress(_work_thread_group->io_service(), address, endpoint);
}

ClientStreamPtr ClientImpl::FindOrCreateStream(const Endpoint& remote_endpoint)
{
    /*
    ClientStreamPtr stream;
    bool create = false;
    {
        ScopedLocker<FastLock> _(_stream_map_lock);
        StreamMap::iterator find = _stream_map.find(remote_endpoint);
        if (find != _stream_map.end() && !find->second->is_closed())
        {
            stream = find->second;
        }
        else
        {
            stream.reset(new ClientStream(_work_thread_group->io_service(), remote_endpoint));
            stream->set_flow_controller(_flow_controller);
            stream->set_max_pending_buffer_size(_max_pending_buffer_size);
            stream->reset_ticks((ptime_now() - _epoch_time).ticks(), true);
            stream->set_connect_timeout(_options.connect_timeout);
            stream->set_closed_stream_callback(
                    boost::bind(&ClientImpl::OnClosed, shared_from_this(), _1));

            _stream_map[remote_endpoint] = stream;
            create = true;
        }
    }
    if (create)
    {
        stream->async_connect();
    }
    return stream;
    */
}

void ClientImpl::OnClosed(const ClientStreamPtr& stream)
{
    /*
    if (!_is_running)
        return;

    ScopedLocker<FastLock> _(_stream_map_lock);
    // to prevent erasing the new connection
    StreamMap::iterator find = _stream_map.find(stream->remote_endpoint());
    if (find != _stream_map.end() && !find->second->is_closed())
    {
        return;
    }
    _stream_map.erase(stream->remote_endpoint());
    */
}

void ClientImpl::StopStreams()
{
    /*
    ScopedLocker<FastLock> _(_stream_map_lock);
    for (StreamMap::iterator it = _stream_map.begin();
            it != _stream_map.end(); ++it)
    {
        it->second->close("client stopped");
    }
    */
}

void ClientImpl::ClearStreams()
{
    ScopedLocker<FastLock> _(_stream_map_lock);
    _stream_map.clear();
}

void ClientImpl::DoneCallback(google::protobuf::Message* response,
        const ControllerImplPtr& cntl)
{
    // erase from TimeoutManager
    _timeout_manager->erase(cntl->TimeoutId());

    // deserialize response
    if (!cntl->Failed())
    {
        //SCHECK(response != NULL);
        //SCHECK(cntl->ResponseBuffer());
        ReadBufferPtr buffer = cntl->ResponseBuffer();
        //CompressType compress_type = cntl->ResponseCompressType();
        bool parse_response_return = false;
        if (compress_type == CompressTypeNone)
        {
            parse_response_return = response->ParseFromZeroCopyStream(buffer.get());
        }
        else
        {
            //hdcs::networking::scoped_ptr<AbstractCompressedInputStream> is(
            //        get_compressed_input_stream(buffer.get(), compress_type));
            //parse_response_return = response->ParseFromZeroCopyStream(is.get());
        }
        if (!parse_response_return)
        {
            //LOG(ERROR) << "DoneCallback(): " << EndpointToString(cntl->RemoteEndpoint())
            //           << ": parse response message pb failed";
            cntl->SetFailed(HDCS_NETWORK_ERROR_PARSE_RESPONSE_MESSAGE, "parse response message pb failed");
        }
    }
}

// this function will actively be called...sdh
void ClientImpl::TimerMaintain(const PTime& now)
{
    //FUNCTION_TRACE;

    int64_t now_ticks = (now - _epoch_time).ticks();

    // checks which need iterator streams
    if (_keep_alive_ticks != -1 || _slice_quota_in != -1 || _slice_quota_out != -1)
    {
        StreamMap streams;
        {
            ScopedLocker<FastLock> _(_stream_map_lock);
            streams = _stream_map;
        }

        // check keep alive time
        if (_keep_alive_ticks != -1)
        {
            for (StreamMap::iterator it = streams.begin(); it != streams.end(); ++it)
            {
                const ClientStreamPtr& stream = it->second;
                if (stream->is_closed())
                {
                    continue;
                }
                if (now_ticks - stream->last_rw_ticks() >= _keep_alive_ticks
                        && stream->pending_process_count() == 0)
                {
                    stream->close("keep alive timeout: "
                                  + boost::lexical_cast<std::string>(_options.keep_alive_time)
                                  + " seconds");
                }
                else
                {
                    stream->reset_ticks(now_ticks, false);
                }
            }
        }

        // flow control in
        if (_slice_quota_in != -1)
        {
            // recharge quota pool
            //_flow_controller->recharge_read_quota(_slice_quota_in);

            // collect streams need to trigger
            std::vector<FlowControlItem> trigger_list;
            trigger_list.reserve(streams.size());
            for (StreamMap::iterator it = streams.begin(); it != streams.end(); ++it)
            {
                const ClientStreamPtr& stream = it->second;
                if (stream->is_closed())
                {
                    continue;
                }
                int token = stream->read_quota_token();
                if (token <= 0)
                {
                    // only need trigger streams whose token <= 0
                    trigger_list.push_back(FlowControlItem(token, stream.get()));
                }
            }

            // sort by token: token closer to zero, earlier to trigger
            std::sort(trigger_list.begin(), trigger_list.end());

            // trigger in order
            for (std::vector<FlowControlItem>::iterator t_it = trigger_list.begin();
                    t_it != trigger_list.end(); ++t_it)
            {
                t_it->stream->trigger_receive();
            }
        }

        // flow control out
        if (_slice_quota_out != -1)
        {
            // recharge quota pool
            //_flow_controller->recharge_write_quota(_slice_quota_out);

            // collect streams need to trigger
            std::vector<FlowControlItem> trigger_list;
            trigger_list.reserve(streams.size());
            for (StreamMap::iterator it = streams.begin(); it != streams.end(); ++it)
            {
                const ClientStreamPtr& stream = it->second;
                if (stream->is_closed())
                {
                    continue;
                }
                int token = stream->write_quota_token();
                if (token <= 0)
                {
                    // only need trigger streams whose token <= 0
                    trigger_list.push_back(FlowControlItem(token, stream.get()));
                }
            }

            // sort by token: token closer to zero, earlier to trigger
            std::sort(trigger_list.begin(), trigger_list.end());

            // trigger in order
            for (std::vector<FlowControlItem>::iterator t_it = trigger_list.begin();
                    t_it != trigger_list.end(); ++t_it)
            {
                t_it->stream->trigger_send();
            }
        }
    }

    _last_maintain_ticks = now_ticks;
}

uint64_t ClientImpl::GenerateSequenceId()
{
    return static_cast<uint64_t>(++_next_request_id);
}
} // 
} // 

