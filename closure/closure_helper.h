#ifndef CLOSURE_HELPER_H_
#define CLOSURE_HELPER_H_

namespace hdcs {
namespace networking {

template <typename T>
inline T * get_pointer(T * p)
{
    return p;
}

// delete p in dtor automatically if Enabled is true
template <bool Enabled, typename T>
class ConditionalAutoDeleter
{
public:
    explicit ConditionalAutoDeleter(T* p)
        : m_p(p)
    {
    }
    ~ConditionalAutoDeleter()
    {
        if (Enabled)
        {
            delete m_p;
        }
    }
private:
    ConditionalAutoDeleter(const ConditionalAutoDeleter&);
    ConditionalAutoDeleter& operator=(const ConditionalAutoDeleter&);
private:
    T* m_p;
};

// This is a typetraits object that's used to take an argument type, and
// extract a suitable type for storing and forwarding arguments.
template <typename T>
struct ParamTraits
{
    typedef const T& ForwardType;
    typedef T StorageType;
};

template <typename T>
struct ParamTraits<T&>
{
    typedef T& ForwardType;
    typedef T StorageType;
};

} // namespace 
} // namespace 

#endif 
