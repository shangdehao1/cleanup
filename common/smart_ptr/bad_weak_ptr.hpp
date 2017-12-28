#ifndef BAD_WEAK_PTR_
#define BAD_WEAK_PTR_

#include <exception>

namespace hdcs {
namespace networking {

class bad_weak_ptr: public std::exception
{
public:
    virtual char const * what() const throw()
    {
        return "hdcs::networking::bad_weak_ptr";
    }
};

} 
} 

#endif
