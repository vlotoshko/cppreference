#include <iostream>
#include <thread>
#include <mutex>
#include "multithreading.hpp"


static thread_local int counter = 0;
std::mutex cout_mutex;
std::timed_mutex cout_timed_mutex;
std::recursive_mutex rec_timed_mutex;

class CallAbleMutexTest
{
public:
    CallAbleMutexTest(std::string name) : threadName_(name) {}
    using type = void;

    template<typename Type = int>
    void operator()(int a, Type t);

private:
    void someCircle(int a)
    {
        for (int i = 0; i < a; ++i)
        {
            std::cout << threadName_<< ": " << std::dec << ++counter <<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
    std::string threadName_;
};

template<> void CallAbleMutexTest::operator()(int a, int)
{
    cout_mutex.lock();
//    cout_mutex.lock();
    someCircle(a);
    cout_mutex.unlock();
}

template<> void CallAbleMutexTest::operator()(int a, bool)
{
    using ms = std::chrono::microseconds;
    if (cout_timed_mutex.try_lock_for(ms(100)))
    {
        someCircle(a);
        std::this_thread::sleep_for(ms(100));
        cout_timed_mutex.unlock();
    }
    else
    {
        std::cout << threadName_ << ": failed to try lock timed mutex" << std::endl;
        std::this_thread::sleep_for(ms(2000));
        if (cout_timed_mutex.try_lock_for(ms(100)))
        {
            someCircle(a);
            cout_timed_mutex.unlock();
        }
        else
        {
            std::cout << threadName_ << ": failed again to try lock timed mutex" << std::endl;
        }
    }
}

template<> void CallAbleMutexTest::operator()(int a, char)
{
    rec_timed_mutex.lock();
    rec_timed_mutex.lock();
    someCircle(a);
    rec_timed_mutex.lock();
    rec_timed_mutex.lock();
}

class GuardedThread : public std::thread
{
public:
    using std::thread::thread;
    ~GuardedThread() { if (joinable()) join(); }
};

void mutexTest()
{
    // Simple mutex
    auto repeat = 3;
    std::thread t{CallAbleMutexTest{"mutex"}, repeat, int{}};
    std::thread t2{CallAbleMutexTest{"mutex"}, repeat, int{}};
    t.join();
    t2.join();

    // Timed mutex
    std::thread t3{CallAbleMutexTest{"timed mutex 1"}, repeat, bool{}};
    std::thread t4{CallAbleMutexTest{"timed mutex 2"}, repeat, bool{}};
    t3.join();
    t4.join();

    // Recursive mutex
    std::thread t6{CallAbleMutexTest{"recursive mutex"}, repeat, char{}};
    t6.join();

    // Guarded mutex
    GuardedThread gT{[](){ std::cout << "guarded thread processing" << std::endl; }};
}

// t.get_id() == std::thread::id{} when thread was:
// created without task
// terminated
// detached
// moved

void threadId()
{
    std::thread t(CallAbleMutexTest{""}, 0, int{});
    std::thread t1; // without task
    if (t.get_id() == std::thread::id{})
    {
        std::cout << "thread t is not joinable" << std::endl;
    }
    else
    {
        t.join();
    }
    if (t1.get_id() == std::thread::id{})
    {
        std::cout << "thread t1 is not joinable" << std::endl;
    }
}

void testMultiThreading()
{
    std::cout << "supported concurrent threads: " << std::thread::hardware_concurrency() <<std::endl;
    mutexTest();
    threadId();
}
