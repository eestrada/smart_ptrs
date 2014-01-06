#include <iostream>
#include "memory.hpp"

int main(void)
{
    int *p = new int();
    std::cerr << "p equals: " << p << std::endl;

    delete p; p = nullptr;
    std::cerr << "p equals: " << p << std::endl;
    return 0;
}
