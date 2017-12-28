#ifndef COUNTER_H_
#define COUNTER_H_

#include "atomic.h"

namespace hdcs {
namespace networking {

class BasicCounter
{
public:
    BasicCounter() : _counter(0) {}
    BasicCounter(uint32_t init) : _counter(init) {}
    uint32_t operator ++ ()
    {
        return ++ _counter;
    }
    uint32_t operator -- ()
    {
        return -- _counter;
    }
    operator uint32_t () const
    {
        return _counter;
    }
private:
    uint32_t _counter;
};

class AtomicCounter
{
public:
    AtomicCounter() : _counter(0) {}
    AtomicCounter(uint32_t init) : _counter(init) {}
    uint32_t operator ++ ()
    {
        return atomic_inc_ret_old(&_counter) + 1U;
    }
    uint32_t operator -- ()
    {
        return atomic_dec_ret_old(&_counter) - 1U;
    }
    operator uint32_t () const
    {
        return _counter;
    }
private:
    volatile uint32_t _counter;
};

class AtomicCounter64
{
public:
    AtomicCounter64() : _counter(0) {}
    AtomicCounter64(uint64_t init) : _counter(init) {}
    uint64_t operator ++ ()
    {
        return atomic_inc_ret_old64(&_counter) + 1LU;
    }
    uint64_t operator -- ()
    {
        return atomic_dec_ret_old64(&_counter) - 1LU;
    }
    operator uint64_t () const
    {
        return _counter;
    }
private:
    volatile uint64_t _counter;
};

} 
}

#endif
