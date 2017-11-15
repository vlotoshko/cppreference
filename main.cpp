#include <iostream>
#include <array>
#include <vector>

#include "initialization.h"
#include "strings.h"
#include "nvi_and_crtp.h"
#include "type_deduction.h"
#include "smart_pointers.h"
#include "other.h"

using std::cout;
using std::endl;


class Moveable
{
public:
    // --- Constructors/destructors
    Moveable() : _value(0) { cout << "default constructor" << endl; }
    explicit Moveable(int a) : _value(a) { cout << "1-parametered constructor" << endl; }
    ~Moveable() {}

    // --- Copy semantics
    Moveable(const Moveable& rhs)
    {
        cout << "copy constructor" << endl;
        _value = rhs._value;
    }
    Moveable& operator =(const Moveable& other)
    {
        cout << "copy assing operator" << endl;
        if(this != &other)
        {
            _value = other._value;
        }
        return *this;
    }

    // --- Move semantics
    Moveable(Moveable&& rhs) noexcept(false) : _value(std::move(rhs._value))
    {
        cout << "move constructor" << endl;
        if(_value > 10)
            throw std::exception();
    }

    Moveable& operator =(Moveable&& other)
    {
        cout << "move assing operator" << endl;
        if(this != &other)
        {
            _value = std::move(other._value);
        }
        return *this;
    }

    // --- Other operations
    Moveable& setValue(int v)
    {
        _value = v;
        return *this;
    }

    friend std::ostream& operator <<(std::ostream& o, const Moveable& m);
private:
    int _value;
};

std::ostream& operator <<(std::ostream& o, const Moveable& m)
{
    o << m._value;
    return o;
}

void processValue(Moveable &)
{
    cout << "processValue invoked for an lvalue" << endl;
}

void processValue(Moveable &&)
{
    cout << "processValue invoked for an rvalue" << endl;
}

template<typename T>
T&& custom_forward(T& t)
{
    return static_cast<T&&>(t);
}

template<typename T>
void processValueT(T&& t)
{
    processValue(std::forward<T>(t));
    processValue(custom_forward<T>(t));
}

void testMoveSemantics()
{
    Moveable m1; // default constructor
    m1.setValue(5);
    auto m2 = m1; // copy constructor
    m2 = m1; // copy assing operator
    auto m3(std::move(m1)); // move constructor
    auto m4 = std::move(m1); // move constructor
    m4 = std::move(m1); // move assing operator

    processValueT(m2); // lvalue
    processValueT(std::move(m1)); // rvalue
    cout << "m1 = " << m1 << endl;

    cout << endl << "have fun with vector" << endl;
    std::vector<Moveable> v;
    v.reserve(3);
    v.push_back(m1);
    v.emplace_back(Moveable());
    v.emplace_back(7);
    v.emplace_back(m1);
}


class TestNew
{
public:
    TestNew(int i) : mInt(i) {}
    int mInt;
};

void testNew()
{
    TestNew * buff = static_cast<TestNew *>(::operator new (sizeof(TestNew) * 10));
    TestNew * n1 = new(buff) TestNew(45);
    TestNew * n5 = new(buff + 5) TestNew(51);
    n5 = buff;
    for (int var = 0; var < 5; ++var)
    {
        n5++;
    }
    cout << n1->mInt << endl;
    cout << n5->mInt << endl;
    n1->~TestNew();
    n5->~TestNew();
    delete buff;
}


int main()
{
  cout << "Hello World!" << endl;
  testMoveSemantics();
//  testOther();
//  testInitalization();
//  testNVI();
//  testCRTP();
//  testTypeDeduction();
//  testSmartPointers();
//  testNew();
//  testRValue();

  return 0;
}
