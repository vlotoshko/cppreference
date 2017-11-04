#include <iostream>
#include <array>

#include "initialization.h"
#include "strings.h"
#include "nvi_and_crtp.h"
#include "type_deduction.h"
#include "smart_pointers.h"
#include "other.h"

using std::cout;
using std::endl;


int passRValue(int&& i)
{
    cout << i++ << endl;
    cout << i << endl;
    return 0;
}

void testRValue()
{
    passRValue(3 + 2);
    passRValue(3);
    int i = 1;
    passRValue(std::move(i));

    int a = 9;
    int& la = a;
    int&& ra = passRValue(7);
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
  std::cout << "Hello World!" << std::endl;
  testOther();
//  testInitalization();
//  testNVI();
//  testCRTP();
//  testTypeDeduction();
//  testSmartPointers();
//  testNew();
//  testRValue();

  return 0;
}
