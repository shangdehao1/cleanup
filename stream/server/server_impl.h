#ifndef SERVER_IMPL_H_
#define SERVER_IMPL_H_

#include <set>

namespace hdcs {
namespace networking {

class ServerImpl : public hdcs::networking::enable_shared_from_this<ServerImpl>
{
public:
    static const int MAINTAIN_INTERVAL_IN_MS = 100;

public:
    ServerImpl(const ServerOptions& options,
                  Server::EventHandler* handler);

    virtual ~ServerImpl();

    bool Start(const std::string& server_address);

    void Stop();

    PTime GetStartTime();

    ServerOptions GetOptions();

    void ResetOptions(const ServerOptions& options);

    bool RegisterService(google::protobuf::Service* service, bool take_ownership);

    int ServiceCount();

    int ConnectionCount();

    void GetPendingStat(int64_t* pending_message_count,
            int64_t* pending_buffer_size, int64_t* pending_data_size);

    bool IsListening();

    // Restart the listener.  
    // It will restart listening anyway, even if it is already in
    // listening.  Return false if the server is not started, or fail to restart listening.
    bool RestartListen();

    //WebServicePtr GetWebService();

    //bool RegisterWebServlet(const std::string& path, Servlet servlet, bool take_ownership);

    //Servlet UnregisterWebServlet(const std::string& path);

private:
    void OnCreated(const ServerStreamPtr& stream);

    void OnAccepted(const ServerStreamPtr& stream);

    void OnAcceptFailed(ErrorCode error_code, const std::string& error_text);

    void OnReceived(const ServerStreamWPtr& stream, const RequestPtr& request);

    void OnClosed(const ServerStreamPtr& stream);

    void StopStreams();

    void ClearStreams();

    void TimerMaintain(const PTime& now);

private:

    /*
    struct FlowControlItem
    {
        int token; // always <= 0
        ServerStream* stream;

        FlowControlItem(int t, ServerStream* s) : token(t), stream(s) {}
        // comparator: nearer to zero, higher priority
        bool operator< (const FlowControlItem& o) const
        {
            return token > o.token;
        }
    };
    */

private:
    ServerOptions _options;
    Server::EventHandler* _event_handler;
    volatile bool _is_running;
    MutexLock _start_stop_lock;

    PTime _start_time;
    int64_t _ticks_per_second;
    int64_t _last_maintain_ticks;
    int64_t _last_restart_listen_ticks;
    int64_t _last_switch_stat_slot_ticks;
    int64_t _last_print_connection_ticks;

    int64_t _slice_count;
    int64_t _slice_quota_in;
    int64_t _slice_quota_out;
    int64_t _max_pending_buffer_size;
    int64_t _keep_alive_ticks;
    int64_t _restart_listen_interval_ticks;
    int64_t _switch_stat_slot_interval_ticks;

    ServicePoolPtr _service_pool;

    //FlowControllerPtr _flow_controller;

    ThreadGroupImplPtr _maintain_thread_group;

    std::string _server_address; // endpoint
    Endpoint _listen_endpoint;
    ListenerPtr _listener;        //  

    TimerWorkerPtr _timer_worker;

    typedef std::set<ServerStreamPtr> StreamSet; // server_stream
    StreamSet _stream_set;
    FastLock _stream_set_lock;

    IOServicePoolPtr _io_service_pool;   //io_service_pool

}; 

} 
}

#endif
