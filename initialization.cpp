#include <iostream>
#include <array>

#include "initialization.h"

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
//  std::array<int, d> a2;    // error: d is not a constant expression
}


// --- zero initialization




void initalization(int argc, char **argv)
{
  std::cout << "Initialization func" << std::endl;

  constinit();
}
