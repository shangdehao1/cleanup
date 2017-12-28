// This file is modified from boost.
//
// Copyright Beman Dawes 2002, 2006
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See library home page at http://www.boost.org/libs/system

#ifndef SMART_PTR_DETAIL_SPINLOCK_PT_
#define SMART_PTR_DETAIL_SPINLOCK_PT_

#include <pthread.h>

namespace hdcs {
namespace networking {
namespace detail {

class spinlock
{
public:

    pthread_mutex_t v_;

public:

    bool try_lock()
    {
        return pthread_mutex_trylock( &v_ ) == 0;
    }

    void lock()
    {
        pthread_mutex_lock( &v_ );
    }

    void unlock()
    {
        pthread_mutex_unlock( &v_ );
    }

public:

    class scoped_lock
    {
    private:

        spinlock & sp_;

        scoped_lock( scoped_lock const & );
        scoped_lock & operator=( scoped_lock const & );

    public:

        explicit scoped_lock( spinlock & sp ): sp_( sp )
        {
            sp.lock();
        }

        ~scoped_lock()
        {
            sp_.unlock();
        }
    };
};

} // namespace
} // namespace
} // namespace

#define HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT { PTHREAD_MUTEX_INITIALIZER }

#endif //
