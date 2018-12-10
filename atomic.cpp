#include <iostream>
#include <string>
#include <atomic>
#include <thread>
#include "atomic.hpp"

// ---------------------------------------------------------------
// --- Antomic flag usage ----------------------------------------
// ---------------------------------------------------------------

std::atomic_flag f = ATOMIC_FLAG_INIT;
std::atomic<bool> go(false);

void af1()
{
    while(!go)
    {
        std::this_thread::yield();
    }
    bool first = true;
    while (f.test_and_set(std::memory_order_acquire))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::cout << "af1 waits for unlock..." << std::endl;
        first = false;
    }

    if (first)
    {
        std::cout << "af1 emulates calculation..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        f.clear(std::memory_order_release);
    }
    else
    {
        std::cout << "af1 emulates calculation after wait..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        f.clear(std::memory_order_release);
    }
    std::cout << "af1 calculation done" << std::endl;
}

void af2()
{
    while(!go)
    {
        std::this_thread::yield();
    }
    bool first = true;
    while (f.test_and_set(std::memory_order_acquire)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::cout << "af2 waits for unlock..." << std::endl;
        first = false;
    }

    if (first)
   {
        std::cout << "af2 emulates calculation..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        f.clear(std::memory_order_release);
    }
    else
    {
        std::cout << "af2 emulates calculation after wait..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        f.clear(std::memory_order_release);
    }
    std::cout << "af2 calculation done" << std::endl;
}

void testAtomicFlag()
{
    std::thread a(af1);
    std::thread b(af2);
    go = true;
    a.join();
    b.join();
}

// ---------------------------------------------------------------
// --- Memory order sequentially consistent ----------------------
// ---------------------------------------------------------------

namespace mo_sc
{
std::atomic<bool> x;
std::atomic<bool> y;
std::atomic<int> z;
struct S { int a; int b; int bb; };
std::atomic<S> u;

void write_x()
{
    x.store(true, std::memory_order_seq_cst);
}

void write_y()
{
    y.store(true, std::memory_order_seq_cst);
}

void read_x_then_y()
{
    while(!x.load(std::memory_order_seq_cst));
    if(y.load(std::memory_order_seq_cst))
        ++z;
}

void read_y_then_x()
{
    while(!y.load(std::memory_order_seq_cst));
    if(x.load(std::memory_order_seq_cst))
        ++z;
}

void testSeqCst()
{
    x = false;
    y = false;
    z = 0;

    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);

    a.join();
    b.join();
    c.join();
    d.join();

    std::cout << "z = " << z.load() << std::endl;

    std::cout << "x is " << (x.is_lock_free() ? "" : "NOT ") << "lock-free" << std::endl;
    std::cout << "y is " << (y.is_lock_free() ? "" : "NOT ") << "lock-free" << std::endl;
    std::cout << "z is " << (z.is_lock_free() ? "" : "NOT ") << "lock-free" << std::endl;
//    std::cout << "u is " << (u.is_lock_free() ? "" : "NOT ") << "lock-free" << std::endl;
}

} // namespace mo_sc

// ---------------------------------------------------------------
// --- Release-acquire syncronization -----------------------------
// ---------------------------------------------------------------

namespace mo_ra
{
std::atomic<bool> x;
std::atomic<bool> y;
std::atomic<int> z;

void write_x_then_y ()
{
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_release);
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_acquire));
    if (x.load(std::memory_order_relaxed))
    {
        ++z;
    }
}

void testRelAcqSync()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);

    a.join();
    b.join();

    std::cout << "z = " << z.load() << std::endl;
}

} // namespace mo_ra


// ---------------------------------------------------------------
// --- Memory order consume --------------------------------------
// ---------------------------------------------------------------

namespace mo_co
{
struct X
{
    int i;
    std::string s;
};

std::atomic<X*> p;
std::atomic<int> a(0);

void create_x()
{
    X* x = new X();
    x->i = 42;
    x->s = "hello";
    a.store(99, std::memory_order_relaxed);
    p.store(x, std::memory_order_release);
}

void use_x()
{
    X* x;
    while (!(x=p.load(std::memory_order_acquire)))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "x.i = " << x->i << std::endl;
    std::cout << "x.s = " << x->s << std::endl;
    std::cout << "a = " << a.load(std::memory_order_relaxed) << std::endl;

}

void testConsume()
{
    std::thread a(create_x);
    std::thread b(use_x);

    a.join();
    b.join();
}

} // namespace mo_co


void testAtomic()
{
//     testAtomicFlag();
//     mo_sc::testSeqCst();
     mo_ra::testRelAcqSync();
     mo_co::testConsume();
}
