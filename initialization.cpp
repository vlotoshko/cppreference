#include <iostream>
#include <array>

#include "initialization.h"


// --- default initialization

class DefInit {
public:
  DefInit() {}
  DefInit(int a) : memb(a) {}
  int memb;
};

struct T1 {
  int mem;
};

struct T2
{
    int mem;
    T2() { } // "mem" is not in the initializer list
};

void defaultinit() {
  DefInit d1;
  DefInit d2(3);
  int a;
  static int b;
  std::cout << "DefInit.memb = " << d1.memb << std::endl;
  std::cout << "DefInit.memb = " << d2.memb << std::endl;
  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;
  T1 t1;
  const T2 t2;
  std::cout << "t1.mem = " << t1.mem << std::endl;
  std::cout << "t2.mem = " << t2.mem << std::endl;
}


// --- constant initialization

struct S {
    static const int c;
};

const int d = 10 * S::c; // not a constant expression: S::c has no preceding
                         // initializer, this initialization happens after const

const int S::c = 5;      // constant initialization, guaranteed to happen first

constexpr const int& return50() {
  return S::c;
}


void constinit() {
  std::cout << "d = " << d << '\n';
  std::array<int, S::c> a1; // OK: S::c is a constant expression
//  std::array<int, d> a2;  // error: d is not a constant expression
}


// --- zero initialization

void zeroinit () {
  int a1;
  int a2();
  static int a3;
}


// --- value initalization

void valueinit () {
  std::cout << "int() = " << int() << std::endl;
  std::cout << "int{} = " << int{} << std::endl;
}


void initalization(int argc, char **argv)
{
  std::cout << "Initialization funcs" << std::endl;

  defaultinit();
  constinit();
  zeroinit();
  valueinit ();
}
