#if !defined(__COW_HPP__)
#define __COW_HPP__

/* C++ headers */
#include <stdexcept>
#include <exception>
#include <iostream>
#include <string>
#include <memory>

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
class cow_ptr
{
/*
 * Copy-On-Write pointer. When the type is going to be modified,
 * it makes a new pointer and copies over the old value to the
 * new pointer before modifying it. This makes it easy to be very
 * memory efficient with heavy data types.
 */
public:
    cow_ptr() noexcept : sh_ptr() {}

    cow_ptr(decltype(nullptr)) noexcept : cow_ptr() {}

    template <typename U>
    explicit cow_ptr(U *p) : sh_ptr(p) {}

    T* get() const { return this->sh_ptr.get(); }

    void reset() { this->sh_ptr.reset(); }

    template <typename U>
    void reset(U *p) { this->sh_ptr.reset(p); }

    template <typename U, typename D>
    void reset(U *p, D del) { this->sh_ptr.reset(p, del); }

    template <typename U, typename D, typename Alloc>
    void reset(U *p, D del, Alloc alloc) { this->sh_ptr.reset(p, del, alloc); }

    T& operator*()
    {
        return this->set_ref();
    }

    T* operator->()
    {
        return &(this->set_ref());
    }

    /*
     * If the useage count of the object is greater than 1, cow_ptr will
     * create a copy of the pointed to object and set it to the current value
     * of the cow_ptr, setting its useage count back to 1.
     *
     * Thus, if the object does not need to modified, the get_ref function
     * should be used instead since this will not cause the cow_ptr to
     * duplicate itself unnecessarily.
     */
    T& set_ref()
    {
        std::cerr << "Running NON-const version of operator*.\n";
        if(this->sh_ptr.use_count() > 1)
        {
            std::shared_ptr<T> tmp(new T(*(this->sh_ptr)));      
            this->sh_ptr = tmp;
        }
        if(this->sh_ptr) return *(this->sh_ptr);
        else throw std::runtime_error("Can't derefence null pointer");
    }

    const T& get_ref() const
    {
        std::cerr << "Running CONST version of operator*.\n";
        if(this->sh_ptr) return *(this->sh_ptr);
        else throw std::runtime_error("Can't derefence null pointer");
    }

    explicit operator bool() const noexcept
    {
        return static_cast<bool>(this->sh_ptr);
    }

    long int use_count() const noexcept
    {
        return this->sh_ptr.use_count();
    }

    bool unique() const noexcept
    {
        return this->sh_ptr.unique();
    }

private:
    std::shared_ptr<T> sh_ptr;
};

template <typename T>
cow_ptr<T> make_cow(T &&v)
{
    return cow_ptr<T>(new T(v));
}

} // End namespace ptr

#endif /* End include guard */

