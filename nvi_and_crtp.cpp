#include "nvi_and_crtp.h"
#include <iostream>

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
