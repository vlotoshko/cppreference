#include <iostream>
#include <thread>
#include "multithreading.hpp"

class CallAbleMulti
{
public:
    void operator()()
    {
        std::cout << "print int in an other thread: " << 5 << std::endl;
    }
private:
};

void printInt(int a)
{
    std::cout << "print int in an other thread: " << a << std::endl;
}

void testMultiThreading()
{
//    CallAbleMulti c;
//    std::thread t{printInt, 5};
//    t.join();
}
