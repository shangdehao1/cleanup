// Copyright Beman Dawes 2002, 2006
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See library home page at http://www.boost.org/libs/system

#ifndef SMART_PTR_DETAIL_SPINLOCK_POOL_
#define SMART_PTR_DETAIL_SPINLOCK_POOL_

//
//  spinlock_pool<0> is reserved for atomic<>, when/if it arrives
//  spinlock_pool<1> is reserved for shared_ptr reference counts
//  spinlock_pool<2> is reserved for shared_ptr atomic access
//

#include "spinlock.hpp"

#include <cstddef>

namespace hdcs {
namespace networking {
namespace detail {

template< int I > class spinlock_pool
{
private:

    static spinlock pool_[ 41 ];

public:

    static spinlock & spinlock_for( void const * pv )
    {
        std::size_t i = reinterpret_cast< std::size_t >( pv ) % 41;
        return pool_[ i ];
    }

    class scoped_lock
    {
    private:

        spinlock & sp_;

        scoped_lock( scoped_lock const & );
        scoped_lock & operator=( scoped_lock const & );

    public:

        explicit scoped_lock( void const * pv ): sp_( spinlock_for( pv ) )
        {
            sp_.lock();
        }

        ~scoped_lock()
        {
            sp_.unlock();
        }
    };
};

template< int I > spinlock spinlock_pool< I >::pool_[ 41 ] =
{
    HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, 
    HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, 
    HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, 
    HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, 
    HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, 
    HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, 
    HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, 
    HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT, 
    HDCS_NETWORK_SMART_PTR_DETAIL_SPINLOCK_INIT
};

} // namespace 
} // namespace
} // namespace

#endif // SMART_PTR_DETAIL_SPINLOCK_POOL_
