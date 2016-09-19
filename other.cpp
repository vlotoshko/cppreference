#include <iostream>
#include "other.h"

using std::cout;
using std::endl;


// --- KEYWORD delete

bool isLucky(int number) { return number == 7; }

// deleted overloaded functions
bool isLucky(char) = delete;
bool isLucky(bool) = delete;
bool isLucky(double) = delete;


template<typename T>
void processPointer(T* ptr);

// deleted explicit specialization
template<>
void processPointer<void>(void*) = delete;
template<>
void processPointer<char>(char*) = delete;
template<>
void processPointer<const void>(const void*) = delete;
template<>
void processPointer<const char>(const char*) = delete;


// -- copy constructor and assignment operator with non const arguments like in the auto_ptr

class NonConstArgs {
private:
public:
  NonConstArgs() {cout << "def constr" << endl;}
  NonConstArgs(int a) {}
  // copy constructor with non const argument
  NonConstArgs(NonConstArgs& rhs) {cout << "copy constr" << endl;}
  // assignment operator with non const argument
  NonConstArgs& operator =(NonConstArgs& rhs) {cout << "assign oper" << endl; return *this; }


  // consersion operators
  NonConstArgs(char a) {cout << "char constr" << endl;}
  operator char() { cout << "char cast oper" << endl; return 'a'; }
  NonConstArgs& operator =(char a) {cout << "char assign oper" << endl; return *this; }
};


NonConstArgs f1() {
  return NonConstArgs(5);
}


void testNonConst() {
  NonConstArgs t1;
  cout << endl;
  t1 = f1();
}





class Base {
public:
  int b;
  virtual void show() const { cout << "Base: b = " << b << endl; }
};

class Derived : public Base {
public:
  int d;
  void show() const { cout << "Derived: b = " << b << "; d = " << d << endl; }
  void newFunc() { cout << ch << endl; }
  char ch;
};


void baseByValue(Base b) {
  b.show();
}

void derivedByValue(Derived d) {
  d.show();
}


void testSlicing() {
  Base b;
  b.b = 1;
  Derived d;
  d.b = 2;
  d.d = 3;

  baseByValue(d);
  baseByValue(b);

//  derivedByValue(static_cast<Derived>(b));
  derivedByValue(d);

  Base *bp = new Base();
  Derived *dp = (Derived *)bp;
  dp->show();
  Derived &dr = *dp;
  Derived *dp1 = static_cast<Derived *>(bp);
  Derived &dr2 = static_cast<Derived &>(*bp);
  dr.newFunc();
  dr2.newFunc();
  dp1->newFunc();

}

void testOther(){
  testNonConst();
  testSlicing();
  cout << endl;

//   isLucky(true);
}
