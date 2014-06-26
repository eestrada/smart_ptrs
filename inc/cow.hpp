#if !defined(__COW_HPP__)
#define __COW_HPP__

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

#include <memory.hpp>

namespace ptr
{

#if __cplusplus == 201103L

#else

#endif /* End C++11 code */

template <typename T>
class cow_ptr : public std::shared_ptr<T>
{
/*
 * Copy-On-Write pointer. When the type is going to be modified,
 * it makes a new pointer and copies over the old value to the
 * new pointer before modifying it. This makes it easy to be very
 * memory efficient with heavy data types.
 */
public:
    cow_ptr() noexcept : std::shared_ptr<T>() {}

    cow_ptr(decltype(nullptr)) noexcept : cow_ptr() {}

    template <typename U>
    explicit cow_ptr(U *p) : std::shared_ptr<T>(p) {}

    const T& get() const;
    T& set();
};

template <typename T>
cow_ptr<T> make_cow(T &&v)
{
    return cow_ptr<T>(new T(v));
}

} // End namespace ptr

#endif /* End include guard */

