#include <iostream>
#include <typeinfo>
#include <list>
#include <chrono>
#include <map>
#include <mutex>
#include "other.hpp"

using std::cout;
using std::endl;


// --- KEYWORD delete

bool isLucky(int number) { return number == 7; }

// deleted overloaded functions
bool isLucky(char) = delete;
bool isLucky(bool) = delete;
bool isLucky(double) = delete;


template<typename T>
void processPointer(T*) {}

// deleted explicit specialization
template<>
void processPointer<void>(void*) = delete;
template<>
void processPointer<char>(char*) = delete;
template<>
void processPointer<const void>(const void*) = delete;
template<>
void processPointer<const char>(const char*) = delete;
template<>
void processPointer<const int>(const int*) = delete;

void testDeleted()
{
    isLucky(7);
//    isLucky(7.3);
//    isLucky(true);
//    isLucky('a');
    isLucky(static_cast<unsigned char>(7.3));

    int * p = nullptr;
    processPointer(p);

//    char * c = nullptr;
//    processPointer(c);

    const int * cp = nullptr;
//    processPointer(cp);
    processPointer(const_cast<int *>(cp));

}


// --- copy constructor and assignment operator with non const arguments like in the auto_ptr

class NonConstArgs
{
public:
    NonConstArgs() {cout << "def constr" << endl;}
    NonConstArgs(int) { cout << "constr with int" << endl; }
    explicit NonConstArgs(char) { cout << "constr with char" << endl; }

    // copy constructor with non const argument
    NonConstArgs(NonConstArgs&) { cout << "copy constr" << endl; }
    // assignment operator with non const argument
    NonConstArgs& operator =(NonConstArgs&) { cout << "assign oper" << endl; return *this; }

    // consersion operators
    operator char() { cout << "char cast oper" << endl; return 'a'; }
    NonConstArgs& operator =(char) { cout << "char assign oper" << endl; return *this; }
};


NonConstArgs f1()
{
    int a = 5;
//    NonConstArgs n = 5; // TODO: compare error at Windows
//    /home/arete/GitHub/cppreference/other.cpp:78: error: invalid initialization of non-const reference of type ‘NonConstArgs&’ from an rvalue of type ‘NonConstArgs’
//         NonConstArgs n = 5;
//                          ^
//    /home/arete/GitHub/cppreference/other.cpp:65: note:   initializing argument 1 of ‘NonConstArgs::NonConstArgs(NonConstArgs&)’
//         NonConstArgs(NonConstArgs&) { cout << "copy constr" << endl; }
//         ^
//    /home/arete/GitHub/cppreference/other.cpp:61: note:   after user-defined conversion: NonConstArgs::NonConstArgs(int)
//         NonConstArgs(int) { cout << "constr with int" << endl; }
//         ^

    return NonConstArgs(a);
}

void testNonConstArgs()
{
    f1();
//    NonConstArgs t1;
//    cout << endl;
//    t1 = f1();
}


void Base::show() const { cout << "Base: b = " << b << endl; }
Base::~Base(){}

void Derived::show() const { cout << "Derived: b = " << b << "; d = " << d << endl; }
void Derived::newFunc() { cout << ch << endl; }

void baseByValue(Base b)
{
    b.show();
}

void derivedByValue(Derived d)
{
    d.show();
}


void testSlicing()
{
    Base b;
    b.b = 1;
    Derived d;
    d.b = 2;
    d.d = 3;

    baseByValue(b);
    baseByValue(d);
    derivedByValue(d);
//    derivedByValue(static_cast<Derived>(b));
}

void testUpcasting()
{
    Base *bp = new Base();
    // upcast by pointer
    Derived *dp = static_cast<Derived *>(bp);
    // upcast by reference
    Derived &dr = static_cast<Derived &>(*bp);
    Derived &dr2 = *dp;
    dp->show();
    dp->newFunc();
    dr.newFunc();
    dr2.newFunc();

    Derived *ddp = dynamic_cast<Derived *>(bp);
    if (ddp == nullptr)
    {
        cout << "dynamic upcast is not allowed" << endl;
    }

    try
    {
        Derived ddr = dynamic_cast<Derived &>(*bp);
    }
    catch (std::bad_cast&)
    {
        cout << "dynamic upcast is not allowed" << endl;
    }
    delete bp;
}

/**
 * @class TimeoutedCache
 * @brief The TimeoutedCache template class caches element for some time period.
 * @note Expired elemetnts are remain in cache, only considered as uncached.
 * @tparam T type of element
 * @tparam DurationType type of duration, seconds by default
 */
template <typename T, typename DurationType = std::chrono::seconds>
struct TimeoutedCache
{
    /**
     * @brief The TimeoutedCache constructor constructs empty cache.
     * @param[in] expiresIn defines period of elements expiration
     * @param[in] maxSize max size of the cache
     */
    TimeoutedCache(size_t expiresIn, size_t maxSize = 10)
        : expirePeriod_(std::move(expiresIn)), maxSize_(std::move(maxSize)) {}

    /**
     * @brief Puts element into the cache.
     * @param[in] element element
     */
    void put(T element)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        auto ret =
                cache_.insert(std::pair<T, std::chrono::system_clock::time_point>(element, std::chrono::system_clock::now()));

        if (ret.second == true) // element added into the map
        {
            orderedByTime_.push_back(std::move(element));
            if (cache_.size() > maxSize_)
            {
                // delete the most expired element from the list
                orderedByTime_.pop_front();
            }
        }
        else // element updated in the map
        {
            // refresh - move element to to the end of the list
            orderedByTime_.remove(element);
            orderedByTime_.push_back(std::move(element));
        }
    }

    /**
     * @brief Check whether exists in the cache.
     * @param[in] element element
     * @return true if element exists in the cache and it is not expired.
     */
    bool exists(const T & element) const
    {
        std::lock_guard<std::mutex> guard(mutex_);
        auto it = cache_.find(element);
        if (it != cache_.end())
        {
            // item 'does not exist' if it was expired
            return std::chrono::duration_cast<DurationType>
                    (std::chrono::system_clock::now() - it->second).count() < expirePeriod_;
        }
        return false;
    }
private:
    /// cache with elements and its time points
    std::map<T, std::chrono::system_clock::time_point> cache_;

    /// elements ordered by time, first is the oldest
    std::list<T> orderedByTime_;

    /// count of periods to expire an element
    size_t expirePeriod_;

    /// max size of the cache
    size_t maxSize_;

    mutable std::mutex mutex_;
};


void testTimeoutedCache()
{
    TimeoutedCache<size_t> lru(10, 1000);
    lru.put(7);

    // sleep fo more that 7 sceonds

    // should return false if 7 seconds was passed
    lru.exists(7);

}


void testOther()
{
    testDeleted();
    testNonConstArgs();
    testSlicing();
    testUpcasting();
    testTimeoutedCache();
    cout << endl;
}
