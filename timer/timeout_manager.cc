
#include "timeout_manager.h"
#include "timeout_manager_impl.h"

namespace hdcs {
namespace networking {

TimeoutManager::TimeoutManager()
{
    _imp.reset(new TimeoutManagerImpl());
    _imp->start();
}

TimeoutManager::~TimeoutManager()
{
    _imp->stop();
    _imp.reset();
}

void TimeoutManager::clear()
{
    _imp->clear();
}

TimeoutManager::Id TimeoutManager::add(int64_t interval, Callback* callback)
{
    return _imp->add(interval, callback);
}

TimeoutManager::Id TimeoutManager::add_repeating(int64_t interval, Callback* callback)
{
    return _imp->add_repeating(interval, callback);
}

bool TimeoutManager::erase(Id id)
{
    return _imp->erase(id);
}

} // namespace networking 
} // namespace hdcs 
