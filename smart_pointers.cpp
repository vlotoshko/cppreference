#include "smart_pointers.h"
#include <iostream>
#include <memory>

using std::cout;
using std::endl;


// --- auto_ptr

// WHY
// excpetion safety, passing ownership of doa into functions, returning doa from functions

using std::auto_ptr;


template<typename T>
void autoPtrByValue(auto_ptr<T> a) {
  cout << *a << endl;
}

template<typename T>
auto_ptr<T> returnAutoPtr(const T a) {
  return auto_ptr<T>(new T(a));
}

struct A {virtual ~A(){}};
struct B : public A {};

void testAutoPtr() {
  // auto_ptr from raw pointer
  int *p1;
  p1 = new int(7);
  auto_ptr<int> a1 (p1);
  std::cout << *a1 << std::endl;

  // auto_ptr from another auto_ptr
  auto_ptr<int> a2 (a1);
  std::cout << *a2 << std::endl;

  // auto_ptr from another auto_ptr related type - type B is convertible to type A
  auto_ptr<B> a3 (new B);
  auto_ptr<A> a4 (a3);

  // assignment operator
  auto_ptr<A> a5(new A);
  a5 = a4;
  auto_ptr<A> a6(new A);
  a6 = a3;

  autoPtrByValue(a2);
  a2 = returnAutoPtr(5);
  // no copy constructor, conversion operator for auto_ptr_ref
  auto_ptr<int> a7 = returnAutoPtr(5);
  std::cout << *a2 << std::endl;
  std::cout << a2.operator ->() << std::endl;
  std::cout << *a2.get() << std::endl;
}



void testSmartPointers() {
  testAutoPtr();
}
