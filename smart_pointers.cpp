#include <iostream>
#include <memory>
#include "smart_pointers.hpp"

using std::cout;
using std::endl;


// --- auto_ptr

// WHY
// excpetion safety, passing ownership of dao into functions, returning dao from functions

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



// --- unigue_ptr

// WHY
// auto_ptr replacement, move semantic instead of 'copy' semantic, user-defined deleter
// array partial specialization
// excpetion safety, passing ownership of dao into functions, returning dao from functions

using std::unique_ptr;


template<typename T, typename D = std::default_delete<T>>
unique_ptr<T, D> getSomeTfromHeap() {
  return unique_ptr<T, D> (new T());
}

// this function has no sence because of const unique_ptr
template<typename T, typename D = std::default_delete<T>>
const unique_ptr<T, D> getSomeConstTfromHeap() {
  return unique_ptr<T, D> (new T());
}

class UniquePtrTest
{
public:
  UniquePtrTest() : _someint(0) { ++_counter; cout << "UniquePtrTest ctor: " << _counter << endl; }
  ~UniquePtrTest() { cout << "UniquePtrTest dtor: " << _counter << endl; --_counter; }
  void incSomeInt() { ++_someint; }
  int getSomeInt() const { return _someint; }
private:
  static int _counter;
  int _someint;
};

int UniquePtrTest::_counter = 0;

std::ostream & operator<< (std::ostream &o,  const UniquePtrTest &u) { return o << 30; }

class UniquePtrTestDeleter {
public:
  void operator()(UniquePtrTest *ut){ cout << "UniquePtrTest dter" << endl; delete ut; }
};


void testUniquePtr() {
  auto u1 = getSomeTfromHeap<int>();
  cout << *u1 << endl;

  auto u2 = getSomeTfromHeap<UniquePtrTest>();
  cout << *u2 << endl;

  auto u3 = getSomeTfromHeap<UniquePtrTest, UniquePtrTestDeleter>();
  cout << *u3 << endl;

  const unique_ptr<UniquePtrTest> u4(new UniquePtrTest());
  cout << *u4 << endl;
  u4->incSomeInt();
  u4->incSomeInt();
  cout << u4->getSomeInt() << endl;
//  auto u4 = getSomeConstTfromHeap<int>();
}


void testSmartPointers() {
  testAutoPtr();
  testUniquePtr();
}
