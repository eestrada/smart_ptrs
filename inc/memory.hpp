#if !defined(__MEMORY_HPP__)
#define __MEMORY_HPP__

/* C++ headers */
#include <stdexcept>
#include <string>
#include <memory>
#include <functional>
#include <unordered_set>
#include <type_traits>

/* C headers */
#include <cstddef>
#include <cstdint>

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

static std::unordered_set<size_t> ptr_set;

/*
 * unique_ptr class interface
 */
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
    constexpr unique_ptr(void) noexcept : ptr(), deleter() {}
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
     * Destructor
     */
    ~unique_ptr(void) noexcept;

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

    /*
     * Member functions.
     */

    pointer get(void) const noexcept;
    deleter_type get_deleter(void) const noexcept;
    explicit operator bool() const noexcept;
    pointer release() noexcept;
    void reset(pointer p = pointer()) noexcept;
    void swap(unique_ptr& other) noexcept;
    element_type& operator*() const; // This CAN throw exceptions
    pointer operator->() const; // This CAN throw exceptions

private:
    pointer ptr;
    deleter_type deleter;
};

/*
 * shared_ptr class interface
 */
class shared_ptr
{
};

/*
 * unique_ptr class implementation
 */
template < typename T, typename D >
unique_ptr<T, D>::unique_ptr(unique_ptr::pointer p) noexcept : ptr(p) {}

template < typename T, typename D >
unique_ptr<T, D>::~unique_ptr(void) noexcept
{
    this->reset();
}

template < typename T, typename D >
typename unique_ptr<T, D>::pointer unique_ptr<T, D>::release() noexcept
{
    pointer p = this->ptr;
    this->ptr = nullptr;
    return p;
}

template < typename T, typename D >
void unique_ptr<T, D>::reset(unique_ptr::pointer p) noexcept
{
    deleter(this->ptr);
    this->ptr = p;
}

template < typename T, typename D >
void unique_ptr<T, D>::swap(unique_ptr& other) noexcept
{
    deleter_type d = other.deleter;
    pointer p = other.ptr;

    other.deleter = this->deleter;
    other.ptr = this->ptr;

    this->deleter = d;
    this->ptr = p;
}

template < typename T, typename D >
typename unique_ptr<T, D>::deleter_type unique_ptr<T, D>::get_deleter(void) const noexcept
{
    return deleter;
}

template < typename T, typename D >
typename unique_ptr<T, D>::pointer unique_ptr<T, D>::get(void) const noexcept
{
    return ptr;
}

template < typename T, typename D >
unique_ptr<T, D>::operator bool(void) const noexcept
{
    return this->get() != nullptr;
}

template < typename T, typename D >
typename unique_ptr<T, D>::element_type& unique_ptr<T, D>::operator*() const // This CAN throw exceptions
{
    if (this->ptr == nullptr) throw ptr_error("Null pointer cannot be dereferenced.");
    else return *ptr;
}

template < typename T, typename D >
typename unique_ptr<T, D>::pointer unique_ptr<T, D>::operator->() const // This CAN throw exceptions
{
    if (this->ptr == nullptr) throw ptr_error("Null pointer cannot be dereferenced.");
    else return ptr;
}

} // End namespace safe

} // End namespace ptr

/*
 * unique_ptr class relational operators
 */

template < typename SP, typename T1, typename D1, typename T2, typename D2>
bool operator == (const ptr::safe::unique_ptr<T1, D1>& lhs, const ptr::safe::unique_ptr<T2, D2>& rhs) noexcept
{
    return uintptr_t(lhs.get()) == uintptr_t(rhs.get());
}

template < typename T, typename D >
bool operator == (const ptr::safe::unique_ptr<T, D>& lhs, std::nullptr_t) noexcept
{
    return uintptr_t(lhs.get()) == uintptr_t(nullptr);
}

template < typename T, typename D >
bool operator == (std::nullptr_t, const ptr::safe::unique_ptr<T, D>& rhs) noexcept
{
    return rhs == nullptr;
}

template < typename T1, typename D1, typename T2, typename D2>
bool operator < (const ptr::safe::unique_ptr<T1, D1>& lhs, const ptr::safe::unique_ptr<T2, D2>& rhs) noexcept
{
    return uintptr_t(lhs.get()) < uintptr_t(rhs.get());
}

template < typename T, typename D >
bool operator < (const ptr::safe::unique_ptr<T, D>& lhs, std::nullptr_t) noexcept
{
    return uintptr_t(lhs.get()) < uintptr_t(nullptr);
}

template < typename T, typename D >
bool operator < (std::nullptr_t, const ptr::safe::unique_ptr<T, D>& rhs) noexcept
{
    return uintptr_t(nullptr) < uintptr_t(rhs.get());
}

/*
 * General relational ops
 * These should work for almost any two types that have == and < defined for them.
 */

template < typename T, typename U >
bool operator != (const T& lhs, const U& rhs) noexcept
{
    return !(lhs == rhs);
}

template < typename T, typename U >
bool operator > (const T& lhs, const T& rhs) noexcept
{
    return rhs < lhs;
}

template < typename T, typename U >
bool operator <= (const T& lhs, const T& rhs) noexcept
{
    return !(rhs < lhs);
}

template < typename T, typename U >
bool operator >= (const T& lhs, const T& rhs) noexcept
{
    return !(lhs < rhs);
}

#endif /* End include guard */

