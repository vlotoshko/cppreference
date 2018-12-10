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
#include "multithreading.hpp"
#include "atomic.hpp"
#include "copy_elision.h"

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
//  testAtomic();
//  testMultiThreading();
//  testLambda();
//  testBinds();
//  testMoveSemantics();
//  testOther();
//  testInitalization();
//  testCopyElision();
//  testNVI();
//  testCRTP();
//  testString();
//  testTypeDeduction();
//  testSmartPointers();
//  testNew();
//  testRValue();
  testOneOfThreads();

  return 0;
}


/*
 * ----- initialization -----------------
 * C++1998
 * default initialization
 * zero initialization
 *
 * constant initialization
 * direct initialization
 * copy initialization
 * reference initialization
 *
 * C++2003
 * value initialization
 *
 * C++2011
 * list initialization
 * agregate initialization
 *
 *
 *
*/
