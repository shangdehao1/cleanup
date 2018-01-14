#ifndef BUF_POOL_H_
#define BUF_POOL_H_

#include "../common/atomic.h"

// base_block_size = 1K
#ifndef TRAN_BUF_BLOCK_BASE_SIZE
#define TRAN_BUF_BLOCK_BASE_SIZE (1024u)
#endif

// max_block_size = 1024<<5 = 32K
#ifndef TRAN_BUF_BLOCK_MAX_FACTOR
#define TRAN_BUF_BLOCK_MAX_FACTOR 5
#endif

namespace hdcs {
namespace networking {

// Reference counted tran buf pool.
//
//   Block = BlockSize(int) + RefCount(int) + Data(char[capacity])
//
// Thread safe.
class TranBufPool
{
public:
    inline static void * malloc(int factor = 0)
    {
        void * p = ::malloc(TRAN_BUF_BLOCK_BASE_SIZE << factor);
        if (p != NULL)
        {
            *(reinterpret_cast<int*>(p)) = TRAN_BUF_BLOCK_BASE_SIZE << factor;
            *(reinterpret_cast<int*>(p) + 1) = 1;
            p = reinterpret_cast<int*>(p) + 2;
        }
        return p;
    }

    // block_size = hidden_header + content_size
    inline static int block_size(void * p)
    {
        return *(reinterpret_cast<int*>(p) - 2);
    }

    // return content_size
    inline static int capacity(void * p)
    {
        return *(reinterpret_cast<int*>(p) - 2) - sizeof(int) * 2;
    }

    // Increase the reference count of the block.
    inline static void add_ref(void * p)
    {
        atomic_inc(reinterpret_cast<int*>(p) - 1);
    }

    // Decrease the reference count of the block.  
    // If the reference count equals
    // to 0 afterward, then put the block back to the free list
    // * The block pointed by "p" was allocated by this pool and is in use currently.
    inline static void free(void * p)
    {
        if (atomic_dec_ret_old(reinterpret_cast<int*>(p) - 1) == 1)
        {
            ::free(reinterpret_cast<int*>(p) - 2);
        }
    }
};

} 
}

#endif 
