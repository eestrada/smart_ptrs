#include <iostream>
#include <memory>
#include <csignal>
#include "memory.hpp"

extern "C" void segv_handler(int parm)
{
    std::cerr << "\n\tA signal is raised from a segmentation fault (which we have caught), " << std::endl;
    std::cerr << "\tbut signals are much harder to recover from gracefully than exceptions." << std::endl;
    std::cerr << "\tWe will terminate now." << std::endl;
    std::exit(1);
}

using ptr::safe::unique_ptr;

int main(void)
{
    std::signal(SIGSEGV, segv_handler);
    std::signal(SIGILL, segv_handler);

    unique_ptr<int> p(new int(100));
    std::unique_ptr<int> q(new int(100));

    std::cerr << "p is a ptr::safe::unique_ptr" << std::endl;
    std::cerr << "q is a std::unique_ptr" << std::endl;

    std::cerr << "p equals: " << p.get() << std::endl;
    std::cerr << "q equals: " << q.get() << std::endl;
    std::cerr << "p's truth value is: " << std::boolalpha << bool(p) << std::endl;
    std::cerr << "q's truth value is: " << std::boolalpha << bool(q) << std::endl;
    std::cerr << "dereferenced p equals: " << *p << std::endl;
    std::cerr << "dereferenced q equals: " << *q << std::endl;

    p.reset(new int(200));
    std::cerr << "p equals: " << p.get() << std::endl;
    std::cerr << "q equals: " << q.get() << std::endl;
    std::cerr << "p's truth value is: " << std::boolalpha << bool(p) << std::endl;
    std::cerr << "q's truth value is: " << std::boolalpha << bool(q) << std::endl;
    std::cerr << "dereferenced p equals: " << *p << std::endl;
    std::cerr << "dereferenced q equals: " << *q << std::endl;

    p.reset();
    q.reset();
    std::cerr << "p equals: " << p.get() << std::endl;
    std::cerr << "q equals: " << q.get() << std::endl;
    std::cerr << "p's truth value is: " << std::boolalpha << bool(p) << std::endl;
    std::cerr << "q's truth value is: " << std::boolalpha << bool(q) << std::endl;

    try
    {
        std::cerr << "dereferenced p creates an exception: " << *p << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << "\n\tException caught: " << e.what() << std::endl;
        std::cerr << "\tContinuing execution." << std::endl;
    }

    std::cerr << "dereferenced q creates a segmentation fault: " << *q << std::endl;


    return 0;
}
