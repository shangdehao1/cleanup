#ifndef MESSAGE_HEADER_H_
#define MESSAGE_HEADER_H_

// message = message_head + meta + data

namespace hdcs {
namespace networking {

// Magic string in little endian.
#define HDCS_MAGIC 1095126867u

// total 24 bytes
struct MessageHeader {
    union {
        char    magic_str[4];
        uint32_t  magic_str_value;
    };                      // 4 bytes
    int32_t   meta_size;    // 4 bytes
    int64_t   data_size;    // 8 bytes
    int64_t   message_size; // 8 bytes: message_size = meta_size + data_size, for check

    MessageHeader()
        : magic_str_value(HDCS_MAGIC)
        , meta_size(0), data_size(0), message_size(0) {}

    bool CheckMagicString() const
    {
        return magic_str_value == HDCS_MAGIC;
    }
};

}
}
#endif
