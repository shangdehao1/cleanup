#ifndef BUF_HANDLE_H_
#define BUF_HANDLE_H_

namespace hdcs {
namespace networking {

struct BufHandle{
    // using tran_buf_pool to malloc space...sdh 
    char* data; // block header.
    int   size; // data size
    union {
        int capacity; // block capacity, used by WriteBuffer
        int offset;   // start position in the block, used by ReadBuffer
    };

    BufHandle(char* _data, int _capacity)
        : data(_data)
        , size(0)
        , capacity(_capacity) {}

    BufHandle(char* _data, int _size, int _offset)
        : data(_data)
        , size(_size)
        , offset(_offset) {}
};

} 
} 
#endif 
