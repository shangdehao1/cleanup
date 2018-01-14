
#include <google/protobuf/stubs/common.h>

#include "thread_group.h"
#include "thread_group_impl.h"

namespace hdcs {
namespace networking {

ThreadGroup::ThreadGroup(int thread_num)
{
    _imp.reset(new ThreadGroupImpl(thread_num));
    _imp->start();
}

ThreadGroup::~ThreadGroup()
{
    _imp->stop();
    _imp.reset();
}

int ThreadGroup::thread_num() const
{
    return _imp->thread_num();
}

void ThreadGroup::dispatch(google::protobuf::Closure* handle)
{
    _imp->dispatch(handle);
}

void ThreadGroup::post(google::protobuf::Closure* handle)
{
    _imp->post(handle);
}

void ThreadGroup::dispatch(ExtClosure<void()>* handle)
{
    _imp->dispatch(handle);
}

void ThreadGroup::post(ExtClosure<void()>* handle)
{
    _imp->post(handle);
}

} // 
} //

