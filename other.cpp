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
    NonConstArgs n = 5;
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

void testOther()
{
    testDeleted();
    testNonConstArgs();
    testSlicing();
    testUpcasting();
    cout << endl;
}
