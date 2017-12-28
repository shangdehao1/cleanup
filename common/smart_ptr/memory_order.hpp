#ifndef SMART_PTR_MEMORY_ORDER_
#define SMART_PTR_MEMORY_ORDER_

namespace hdcs {
namespace networking {

//
// Enum values are chosen so that code that needs to insert
// a trailing fence for acquire semantics can use a single
// test such as:
//
// if( mo & memory_order_acquire ) { ...fence... }
//
// For leading fences one can use:
//
// if( mo & memory_order_release ) { ...fence... }
//
// Architectures such as Alpha that need a fence on consume
// can use:
//
// if( mo & ( memory_order_acquire | memory_order_consume ) ) { ...fence... }
//

enum memory_order
{
    memory_order_relaxed = 0,
    memory_order_acquire = 1,
    memory_order_release = 2,
    memory_order_acq_rel = 3, // acquire | release
    memory_order_seq_cst = 7, // acq_rel | 4
    memory_order_consume = 8
};

} // 
} // 

#endif 
