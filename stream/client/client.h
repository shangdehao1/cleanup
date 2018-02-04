#ifndef CLIENT_H_
#define CLIENT_H_

//#include "client_impl.h"

namespace hdcs {
namespace networking {

class ClientImpl;

struct ClientOptions {
    int work_thread_num;         // num of threads used for network handing, default 4.
    int callback_thread_num;     // num of threads used for async callback, default 4.

    int keep_alive_time;         // keep alive time of idle connections.
                                 // idle connections will be closed if no read/write for this time.
                                 // in seconds, should >= -1, -1 means for ever, default -1.

    int max_pending_buffer_size; // max buffer size of the pending send queue for each connection.
                                 // in MB, should >= 0, 0 means no buffer, default 100.

    // Network throughput limit.
    // The network bandwidth is shared by all connections:
    // * busy connections get more bandwidth.
    // * the total bandwidth of all connections will not exceed the limit.
    int max_throughput_in;       // max network in throughput for all connections.
                                 // in MB/s, should >= -1, -1 means no limit, default -1.
    int max_throughput_out;      // max network out throughput for all connections.
                                 // in MB/s, should >= -1, -1 means no limit, default -1.

    // Timeout for connect in milliseconds
    //
    // If it is not set or set no more than 0, it will not consider the timeout
    // ot the connect.
    //
    // default is -1
    int connect_timeout;

    ClientOptions()
        : work_thread_num(4)
        , callback_thread_num(4)
        , keep_alive_time(-1)
        , max_pending_buffer_size(100)
        , max_throughput_in(-1)
        , max_throughput_out(-1)
        , connect_timeout(-1)
    {}
};

class Client
{
public:
    explicit Client(const ClientOptions& options = ClientOptions());
    virtual ~Client();

    // Get the current  client options.
    ClientOptions GetOptions();

    // Reset the  client options.
    //
    // Current only these options can be reset (others will be ignored):
    // * max_pending_buffer_size : will take effective immediately.
    // * max_throughput_in : will take effective from the next time slice (0.1s). 
    // * max_throughput_out : will take effective from the next time slice (0.1s).
    //
    // Though you want to reset only part of these options, the other options also
    // need to be set.  Maybe you can reset by this way:
    //     ClientOptions options = _client->GetOptions();
    //     options.max_throughput_in = new_max_throughput_in; // reset some options
    //     _client->ResetOptions(options);
    //
    // The old and new value of reset options will be print to INFO log.
    void ResetOptions(const ClientOptions& options);

    // Get the count of current alive connections.
    int ConnectionCount();

    // Shutdown the  client.
    void Shutdown();

public:
    const hdcs::networking::shared_ptr<ClientImpl>& impl() const
    {
        return _impl;
    }

private:
    hdcs::networking::shared_ptr<ClientImpl> _impl;
};

} // namespace 
} // namespace 
#endif // 
