#if !defined(__MEMORY_HPP__)
#define __MEMORY_HPP__

#include <stdexcept>
#include <string>
#include <memory>

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

} // End namespace ptr
#endif /* End include guard */
