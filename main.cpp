#include <iostream>
#include "initialization.h"
#include "strings.h"
#include "nvi_and_crtp.h"
#include "type_deduction.h"
#include "smart_pointers.h"
#include "other.h"


using std::cout;
using std::endl;


int main(int argc, char **argv)
{
  std::cout << "Hello World!" << std::endl;
  testOther();
//  testInitalization(argc, argv);
//  testNVI();
//  testCRTP();
//  testTypeDeduction();
  testSmartPointers();

  return 0;
}
