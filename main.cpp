#include <iostream>
#include <typeinfo>
#include "initialization.h"
#include "strings.h"


// --- NVI classes

class BaseNVI {
private:
  virtual void helloWorld_impl () = 0;
public:
  virtual ~BaseNVI(){}
  void helloWorld () {
    std::cout << "check pre-conditions" << std::endl;
    helloWorld_impl();
    std::cout << "check post-conditions" << std::endl;
  }
};


class DerivedNVI : public BaseNVI{
  void helloWorld_impl() {
    std::cout << "derived nvi method" << std::endl;
  }
};


void testNVI() {
  BaseNVI* pd = new DerivedNVI{};
  pd->helloWorld();
  delete pd;
}


// --- CRTP - curiously recurring template pattern

template <class T>
class Base {
public:
  void interface() {
    static_cast<T*>(this)->implementation();
  }
  static void static_func() {
    T::static_sub_func();
  }
};

class DerivedCRTP : public Base<DerivedCRTP> {
public:
  void implementation() {
    std::cout << "derived implementation func invoked" << std::endl;
  }
  static void static_sub_func() {
    std::cout << "T:: static_sub_func invoked" << std::endl;
  }
};

template<typename T>
struct Counter {
  static int objects_created;
  static int objects_alive;

  Counter() {
    ++objects_created;
    ++objects_alive;
  }

  ~Counter() {
    --objects_alive;
  }
};

template <typename T> int Counter<T>::objects_created = 0;
template <typename T> int Counter<T>::objects_alive = 0;

class CounterCRPT : public Counter<CounterCRPT> {
};



void testCRTP() {
  DerivedCRTP d1;
  d1.implementation();
  d1.static_func();
  Base<DerivedCRTP>* d2 = new DerivedCRTP();
  d2->interface();
  d2->static_func();
  delete d2;

  {
    CounterCRPT c1;
  }
  CounterCRPT c2;
  std::cout << "objects created = " << CounterCRPT::objects_created << std::endl;
  std::cout << "objects alive = " << CounterCRPT::objects_alive << std::endl;
}


// --- C++1y - type deduction

// using undefined template to findout type T by compiler error

// paramType is a reference
template<typename T>
void errFunc1(T& param);

// paramType is a universal reference
template<typename T>
void errFunc2(T&& param);

// paramType is nor a reference neither universal reference
template<typename T>
void errFunc3(T param);


template< typename T, std::size_t N >
constexpr std::size_t arraySize(T (&) [N]) {
    return N;
}

void someFunc(double, int) {}

// difference in auto and template type dedution - std::initializer_list

template<typename T>
void ilFunc(std::initializer_list<T> param);


// auto as trailing return type
template <typename Container, typename Index>
auto autoTRTFunc(Container& c, Index i) -> decltype(c[i]) {
    return c[i];
}

// auto uses template type deduction
template <typename Container, typename Index>
auto autoTTDlFunc(Container& c, Index i) {
    return c[i];
}

// decltype(auto) means type should be deducted but using decltype rules
//template <typename Container, typename Index>
//decltype(auto)
//retDeclFunc(Container& c, Index i) {
//    return c[i];
//}




void testTypeDeduction() {
    int x = 27;
    const int cx = x;
    const int& rx = x;

    const char name[] = "array";

//    errFunc1(x);
//    errFunc1(cx);
//    errFunc1(rx);
//    errFunc1(42);
//    errFunc1(name);
//    errFunc1(someFunc);

//    errFunc2(x);
//    errFunc2(cx);
//    errFunc2(rx);
//    errFunc2(42);
//    errFunc2(name);
//    errFunc2(someFunc);

//    errFunc3(x);
//    errFunc3(cx);
//    errFunc3(rx);
//    errFunc3(42);
//    errFunc3(name);
//    errFunc3(someFunc);

    std::cout << arraySize(name) << std::endl;

//    auto a = {27};
//    errFunc3(a); // std::initalizer_list<int>
//    ilFunc({1,2,3});

    auto a1 = rx;
    errFunc1(a1);
//    decltype(auto) a2 = rx;

}



int main(int argc, char **argv)
{
    std::cout << "Hello World!" << std::endl;
//    initalization(argc, argv);
//    testNVI();
//    testCRTP();
    testTypeDeduction();

    return 0;
}
