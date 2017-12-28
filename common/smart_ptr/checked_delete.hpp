#ifndef SMART_PTR_CHECKED_DELETE_
#define SMART_PTR_CHECKED_DELETE_

namespace hdcs {
namespace networking {

template <typename T>
inline void checked_delete(T* px)
{
    typedef char type_must_be_complete[sizeof(T) ? 1 : -1 ];
    (void) sizeof(type_must_be_complete);
    delete px;
}

template<class T> inline void checked_array_delete(T * x)
{
    typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
    (void) sizeof(type_must_be_complete);
    delete [] x;
}

template<class T> struct checked_deleter
{
    typedef void result_type;
    typedef T * argument_type;

    void operator()(T * x) const
    {
        checked_delete(x);
    }
};

template<class T> struct checked_array_deleter
{
    typedef void result_type;
    typedef T * argument_type;

    void operator()(T * x) const
    {
        checked_array_delete(x);
    }
};

} 
}

#endif
