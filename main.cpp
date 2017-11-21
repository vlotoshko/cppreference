#include <iostream>
#include <array>

#include "initialization.hpp"
#include "strings.hpp"
#include "nvi_and_crtp.hpp"
#include "type_deduction.hpp"
#include "smart_pointers.hpp"
#include "other.hpp"
#include "move_semantics.hpp"
#include "lambda.hpp"

using std::cout;
using std::endl;

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
  testLambda();
//  testMoveSemantics();
//  testOther();
//  testInitalization();
//  testNVI();
//  testCRTP();
//  testString();
//  testTypeDeduction();
//  testSmartPointers();
//  testNew();
//  testRValue();

  return 0;
}
