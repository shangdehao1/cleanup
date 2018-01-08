#include <vector>
#include "request.h"
#include "binary_request_parser.h"

namespace hdcs {
namespace networking {

void RequestParser::RegisteredParsers(std::vector<RequestParserPtr>* parsers)
{
    parsers->push_back(RequestParserPtr(new BinaryRequestParser()));
}

} 
}
