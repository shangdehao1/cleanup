#ifndef TIMEOUT_MANAGER_H_
#define TIMEOUT_MANAGER_H_

//#include <hdcs/networking/common.h>
//#include <hdcs/networking/ext_closure.h>
#include "common/smart_ptr/networking_ptr.h"
#include "closure/closure.h"
#include "closure/ext_closure.h"

namespace hdcs {
namespace networking {

class TimeoutManager;
typedef hdcs::networking::shared_ptr<TimeoutManager> TimeoutManagerPtr;

class TimeoutManagerImpl;

class TimeoutManager
{
public:
    // Timeout id.  Every timeout event will be assigned an unique id.
    typedef uint64_t Id;

    // Timeout type.  Used in callback.
    //   - TIMEOUTED : callback triggered when timeout.
    //   - ERASED    : callback triggered when do 'erase()'.
    //   - CLEARED   : callback triggered when do 'clear()' or destructor.
    enum Type {
        TIMEOUTED = 0,
        ERASED = 1,
        CLEARED = 2
    };

    // Callback.  The callback should be a light weight routine, which means you
    // should not do numerous things or block it.
    // param 1 : id
    // parma 2 : type
    typedef ExtClosure<void(Id, Type)> Callback;

    // Contructor.
    TimeoutManager();

    // Destructor.
    // It will invoke clear() to clear all timeout events.
    ~TimeoutManager();

    // Clear all timeout events from the manager.  All associated callbacks will
    // be invoked with type = CLEARED,
    void clear();

    // Add a one-time timeout event that will be triggered once after time of
    // "interval" milli-seconds from now.
    //
    // The "interval" should be no less than 0.
    //
    // The "callback" should be a self delete closure which can be created through
    // NewExtClosure().
    //
    // The "callback" will always be invoked only once, in following cases:
    //   - Timeouted, with type = TIMEOUTED.
    //   - Erased, with type = ERASED.
    //   - Cleared, with type = CLEARED.
    //
    // After callback done, the "callback" closure will be self deleted.
    Id add(int64_t interval, Callback* callback);

    // Add a repeating timeout event that will be triggered after each "interval"
    // milli-seconds.
    //
    // The "interval" should be no less than 0.
    //
    // The "callback" should be a permanent closure which can be created through
    // NewPermanentExtClosure().
    //
    // The "callback" will always be invoked, in following cases:
    //   - Timeouted, with type = TIMEOUTED, maybe invoked for multi-times.
    //   - Erased, with type = ERASED.
    //   - Cleared, with type = CLEARED.
    //
    // If callbacked in case of ERASED or CLEARED, the "callback" closure will
    // deleted by the manager.
    Id add_repeating(int64_t interval, Callback* callback);

    // Erase a given timeout event, returns true if the event was actually erased
    // and false if it didn't exist.
    //
    // If returns true, the associated callback will be invoked with type = ERASED.
    bool erase(Id id);

private:

    hdcs::networking::shared_ptr<TimeoutManagerImpl> _imp;
};

} // namespace networking
} // namespace hdcs

#endif
