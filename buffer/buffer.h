#ifndef BUFFER_H_
#define BUFFER_H_

#include <deque>
#include <memory>
#include <string>

// google protobuf
#include <google/protobuf/io/zero_copy_stream.h>

#include "buf_handle.h"
#include "../common/smart_ptr/smart_ptr.hpp"

namespace hdcs {
namespace networking {

class ReadBuffer;
typedef hdcs::networking::shared_ptr<ReadBuffer> ReadBufferPtr;

class WriteBuffer;
typedef hdcs::networking::shared_ptr<WriteBuffer> WriteBufferPtr;

typedef std::deque<BufHandle> BufHandleList;
typedef std::deque<BufHandle>::iterator BufHandleListIterator;
typedef std::deque<BufHandle>::reverse_iterator BufHandleListReverseIterator;

class ReadBuffer : public google::protobuf::io::ZeroCopyInputStream
//class ReadBuffer
{
public:
    ReadBuffer();
    virtual ~ReadBuffer();

    // Append a buf handle to the buffer.
    //
    // Preconditions:
    // * No method Next(), Backup() or Skip() have been called before.
    // * The size of "buf_handle" should be greater than 0.
    // * For the first one, size of "buf_handle" should be greater than 0.
    // * For the second one, "read_buffer" should not be NULL.
    void Append(const BufHandle& buf_handle);
    void Append(const ReadBuffer* read_buffer);

    // Get the total byte count of the buffer.
    int64_t TotalCount() const;

    // Get the block count occupied by the buffer.
    int BlockCount() const;

    // Get the total block size occupied by the buffer.
    int64_t TotalBlockSize() const;

    // Trans buffer to string.
    std::string ToString() const;

    // implements ZeroCopyInputStream when using zeroCopyInputStream 
    bool Next(const void** data, int* size);
    void BackUp(int count);
    bool Skip(int count);
    int64_t ByteCount() const;

private:
    BufHandleList _buf_list;
    int64_t _total_block_size; // total block size in the buffer
    int64_t _total_bytes; // total bytes in the buffer
    BufHandleListIterator _cur_it;
    int _cur_pos;
    int _last_bytes; // last read bytes
    int64_t _read_bytes; // total read bytes

};

class WriteBuffer : public google::protobuf::io::ZeroCopyOutputStream
//class WriteBuffer
{
public:
    WriteBuffer();
    virtual ~WriteBuffer();

    // Get the total capacity of the buffer.
    int64_t TotalCapacity() const;

    // Get the block count occupied by the buffer.
    int BlockCount() const;

    // Get the total block size occupied by the buffer.
    int64_t TotalBlockSize() const;

    // Swap out data from this output stream and append to the input stream "is".
    // The "is" should not be null.
    //
    // Postconditions:
    // * This buffer is cleared afterward, just as a new output buffer.
    void SwapOut(ReadBuffer* is);

    // Reserve some space in the buffer.
    // If succeed, return the head position of reserved space.
    // If failed, return -1.
    //
    // Preconditions:
    // * "count" > 0
    int64_t Reserve(int count);

    // Set data in the buffer, start from "pos".
    //
    // Preconditions:
    // * "pos" >= 0
    // * "data" != NULL && "size" > 0
    // * "pos" + "size" <= ByteCount()
    void SetData(int64_t pos, const char* data, int size);

    // 
    bool Next(void** data, int* size);

    void BackUp(int count);

    int64_t ByteCount() const;

    // Append data to the buffer
    bool Append(const std::string& data);
    bool Append(const char* data, int size);

private:
    // Add a new block to the end of the buffer.
    bool Extend();

private:
    BufHandleList _buf_list;
    int64_t _total_block_size; // total block size in the buffer
    int64_t _total_capacity; // total capacity in the buffer
    int _last_bytes; // last write bytes
    int64_t _write_bytes; // total write bytes

};

} 
} 

#endif
