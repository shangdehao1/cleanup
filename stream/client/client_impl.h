#ifndef CLIENT_IMPL_H_
#define CLIENT_IMPL_H_

#include <list>
#include <map>

#include "../../common/smart_ptr/networking_ptr.h"
#include "../../controller/controller_impl.h"
#include "../../io_service/thread_group_impl.h"
#include "../../common/lock/locks.h"
#include "../../buffer/buffer.h"
#include "../../data/request/request.h"
#include "client_stream.h"
#include "client.h"

/*
#include <sofa/pbrpc/common_internal.h>
#include <sofa/pbrpc/rpc_client.h>
#include <sofa/pbrpc/rpc_client_stream.h>
#include <sofa/pbrpc/rpc_endpoint.h>
#include <sofa/pbrpc/thread_group_impl.h>
#include <sofa/pbrpc/timer_worker.h>
#include <sofa/pbrpc/rpc_timeout_manager.h>
*/

namespace hdcs {
namespace networking {

class ClientOptions;

class ClientImpl: public hdcs::networking::enable_shared_from_this<ClientImpl>
{
public:
    static const int MAINTAIN_INTERVAL_IN_MS = 100;

public:
    explicit ClientImpl(const ClientOptions& options);

    virtual ~ClientImpl();

    void Start();

    void Stop();

    ClientOptions GetOptions();

    void ResetOptions(const ClientOptions& options);

    int ConnectionCount();

    //  call method to remote endpoint.
    //
    // The call can be done in following cases:
    // * send failed
    // * timeouted
    // * response received
    void CallMethod(const google::protobuf::Message* request,
            google::protobuf::Message* response,
            const ControllerImplPtr& cntl);

    const ThreadGroupImplPtr& GetCallbackThreadGroup() const;

    bool ResolveAddress(const std::string& address,
            Endpoint* endpoint);

private:
    // Get stream for "remote_endpoint".  Return null ptr if failed.
    ClientStreamPtr FindOrCreateStream(const Endpoint& remote_endpoint);

    void OnClosed(const ClientStreamPtr& stream);

    void StopStreams();

    void ClearStreams();

    void DoneCallback(google::protobuf::Message* response,
            const ControllerImplPtr& cntl);

    bool ShouldStreamRemoved(const ClientStreamPtr& stream);

    void TimerMaintain(const PTime& now);

    uint64_t GenerateSequenceId();

private:
    struct FlowControlItem
    {
        int token; // always <= 0
        ClientStream* stream;

        FlowControlItem(int t, ClientStream* s) : token(t), stream(s) {}
        // comparator: nearer to zero, higher priority
        bool operator< (const FlowControlItem& o) const
        {
            return token > o.token;
        }
    };

private:
    ClientOptions _options;
    volatile bool _is_running;
    MutexLock _start_stop_lock;

    AtomicCounter64 _next_request_id;

    PTime _epoch_time;
    int64_t _ticks_per_second;
    int64_t _last_maintain_ticks;
    int64_t _last_print_connection_ticks;

    int64_t _slice_count;
    int64_t _slice_quota_in;
    int64_t _slice_quota_out;
    int64_t _max_pending_buffer_size;
    int64_t _keep_alive_ticks;

    //FlowControllerPtr _flow_controller;

    ThreadGroupImplPtr _maintain_thread_group;
    ThreadGroupImplPtr _callback_thread_group;
    ThreadGroupImplPtr _work_thread_group;

    TimerWorkerPtr _timer_worker;
    TimeoutManagerPtr _timeout_manager;

    typedef std::map<Endpoint, ClientStreamPtr> StreamMap;
    StreamMap _stream_map;
    FastLock _stream_map_lock;

}; 

} // 
} // 

#endif //
