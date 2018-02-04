#include "binary_request_parser.h"

namespace hdcs {
namespace networking {

#define HDCS_NETWORK_MAX_MESSAGE_SIZE (64 << 20) 

const int64_t BinaryRequestParser::MAX_MESSAGE_SIZE = 
    HDCS_NETWORK_MAX_MESSAGE_SIZE + sizeof(MessageHeader);

BinaryRequestParser::BinaryRequestParser() :
    _state(PS_MAGIC_STRING),
    _bytes_recved(0),
    _req(new BinaryRequest())
{
}

BinaryRequestParser::~BinaryRequestParser()
{
}

const char* BinaryRequestParser::Name()
{
    return "BinaryRequestParser";
}

void BinaryRequestParser::Reset()
{
    _state = PS_MAGIC_STRING;
    _bytes_recved = 0;
    _req.reset(new BinaryRequest());
}

bool BinaryRequestParser::CheckMagicString(const char* magic_string)
{
    //SCHECK(_state == PS_MAGIC_STRING && _bytes_recved == 0);
    if (*reinterpret_cast<const uint32_t*>(magic_string) == 
            _req->_req_header.magic_str_value)
    {
        _state = PS_MSG_HEADER;
        _bytes_recved = 4;
        return true;
    }
    return false;
}

int BinaryRequestParser::Parse(const char* buf,
        int data_size, int offset, int* bytes_consumed)
{
    if (data_size == 0)
    {
        return 0;
    }
    *bytes_consumed = 0;
    int64_t bytes_remain, consume;
    switch (_state)
    {
        case PS_MSG_HEADER:
            bytes_remain = sizeof(MessageHeader) - _bytes_recved;
            consume = std::min(static_cast<int64_t>(data_size), bytes_remain);
            memcpy(reinterpret_cast<char*>(&_req->_req_header) + _bytes_recved,
                    buf + offset, consume);
            *bytes_consumed += consume;
            _bytes_recved += consume;
            if (_bytes_recved < static_cast<int>(sizeof(MessageHeader)))
            {
                // header not complete
                return 0;
            }
            if (_req->_req_header.message_size == 0)
            {
                // all complete
                return 1;
            }
            // header complete
            data_size -= consume;
            offset += consume;
            _state = PS_MSG_BODY;
            _bytes_recved = 0; // because message_size not include the header
            if (data_size == 0)
            {
                return 0;
            }
        case PS_MSG_BODY:
            bytes_remain = _req->_req_header.message_size - _bytes_recved;
            if (bytes_remain > MAX_MESSAGE_SIZE || bytes_remain <= 0)
            {
                // compute bytes_remain error
                //SLOG(ERROR, "Parse(): %s: compute bytes_remain[%lld] error, message size limit[%lld]",
                //     EndpointToString(_req->_remote_endpoint).c_str(), 
                //     bytes_remain, MAX_MESSAGE_SIZE);
                return -1;
            }
            consume = std::min(static_cast<int64_t>(data_size), bytes_remain);
            _req->_req_body->Append(BufHandle(const_cast<char*>(buf), consume, offset));
            *bytes_consumed += consume;
            _bytes_recved += consume;
            if (_bytes_recved == _req->_req_header.message_size)
            {
                // receive body complete
                if (!_req->_req_meta.ParseFromBoundedZeroCopyStream(
                            _req->_req_body.get(), _req->_req_header.meta_size))
                {
                     //parse meta failed
                     //SLOG(ERROR, "Parse(): %s: parse rpc meta failed",
                     //       EndpointToString(_req->_remote_endpoint).c_str());
                    return -1;
                }
                return 1;
            }
            return 0;
        default:
            //SCHECK(false);
            return -1;
    }
}

RequestPtr BinaryRequestParser::GetRequest()
{
    return _req;
}

} 
} 
