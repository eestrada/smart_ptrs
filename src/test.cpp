#include <iostream>
#include "memory.hpp"

int main(void)
{
    using ptr::safe::unique_ptr;
    unique_ptr<int> p(new int(100));
    std::cerr << "p equals: " << p.get() << std::endl;
    std::cerr << "p's truth value is: " << std::boolalpha << bool(p) << std::endl;
    std::cerr << "dereferenced p equals: " << *p << std::endl;

    p.reset(new int(200));
    std::cerr << "p equals: " << p.get() << std::endl;
    std::cerr << "p's truth value is: " << std::boolalpha << bool(p) << std::endl;
    std::cerr << "dereferenced p equals: " << *p << std::endl;

    p.reset();
    std::cerr << "p equals: " << p.get() << std::endl;
    std::cerr << "p's truth value is: " << std::boolalpha << bool(p) << std::endl;

    return 0;
}
