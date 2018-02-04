#ifndef BINARY_REQUEST_PARSER_H_
#define BINARY_REQUEST_PARSER_H_

#include "request_parser.h"
#include "binary_request.h"

namespace hdcs {
namespace networking {

class BinaryRequestParser : public RequestParser
{
public:
    BinaryRequestParser();
    virtual ~BinaryRequestParser();

    virtual const char* Name();

    virtual void Reset();

    virtual bool CheckMagicString(const char* magic_string);

    virtual int Parse(const char* buf, int data_size, int offset, int* bytes_consumed);

    virtual RequestPtr GetRequest();

private:
    enum ParseState
    {
        PS_MAGIC_STRING,
        PS_MSG_HEADER,
        PS_MSG_BODY
    };
    ParseState _state; // current parsing state
    int _bytes_recved; // bytes received in current state
    BinaryRequestPtr _req; // request!!!

    static const int64_t MAX_MESSAGE_SIZE;
}; 

} 
}

#endif
