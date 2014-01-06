#if !defined(__MEMORY_HPP__)
#define __MEMORY_HPP__

/* C++ headers */
#include <stdexcept>
#include <string>
#include <memory>
#include <type_traits>

/* C headers */
#include <cstddef>

namespace ptr
{

#if __cplusplus == 201103L

#else

#endif /* End C++11 code */

class ptr_error : public std::runtime_error
{
public:
    explicit ptr_error(const std::string &what_arg) : 
        std::runtime_error(what_arg) {}

    #if __cplusplus == 201103L
    explicit ptr_error(const char *what_arg) :
        std::runtime_error(what_arg) {}
    #endif /* End C++11 code */
};

/*
 * Collection of "safe" pointers. These will not derefence null pointers
 * like the standard library smart pointers will. Thus, these are slightly
 * more expensive, but much more safe as well.
 *
 * However, if a raw pointer is shared with more than one instance of a 
 * safe::unique_ptr, then your program will likely crash. There is no way for
 * unique_ptr or safe::unique_ptr to know you are doing this... so don't do it. 
 * Use a shared_ptr or a safe::shared_ptr instead.
 */
namespace safe
{

template < typename T, typename D = std::default_delete<T> >
class unique_ptr
{
public: // Member types
    typedef T element_type;
    typedef D deleter_type;
    typedef T* pointer;
public:

    /*
     * Constructors
     */
    constexpr unique_ptr() noexcept;
    constexpr unique_ptr (std::nullptr_t) noexcept : unique_ptr() {}
    explicit unique_ptr (pointer p) noexcept;
    unique_ptr (pointer p,
        typename std::conditional<std::is_reference<D>::value,D,
        const D&> del) noexcept;
    unique_ptr (pointer p,
        typename std::remove_reference<D>::type&& del) noexcept;
    unique_ptr (unique_ptr&& x) noexcept;
    template <class U, class E>
      unique_ptr (unique_ptr<U,E>&& x) noexcept;
    /*
     * Never copy! Only move.
     */
    unique_ptr (const unique_ptr&) = delete;

    /*
     * Assignment operators
     */
    unique_ptr& operator=(unique_ptr&& x) noexcept;
    unique_ptr& operator=(std::nullptr_t) noexcept;

    template < typename U, typename E >
    unique_ptr& operator=(unique_ptr<U, E>&& x) noexcept;

    /*
     * Never copy! Only move.
     */
    unique_ptr& operator=(const unique_ptr&) = delete;

private:
    pointer ptr;
    deleter_type deleter;
};

class shared_ptr
{
};

} // End namespace safe

} // End namespace ptr
#endif /* End include guard */
