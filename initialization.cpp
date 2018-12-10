#include <iostream>
#include <array>

#include "initialization.hpp"


// --- default initialization (since 98)

class DefInit
{
public:
  DefInit() {}
  DefInit(int a) : memb(a) {}
  int memb;
};

struct T1
{
  int mem;
};

struct T2
{
    int mem;
    T2() { } // "mem" is not in the initializer list
};

void defaultinit()
{
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


// --- direct & copy initialization

void direct_and_copy_Init()
{
    int x(7);  // direct initialization
    int y = 7; // copy initialization

    // copy-initialization is less permissive than direct-initialization
    // it does not use explicit constructors in the overload resolution
    struct Exp
    {
        explicit Exp(const char*) {}
    };
    Exp e1("abc");  // ok
//    Exp e2 = "abc"; // error, copy-initialization does not consider explicit constructor
}

// --- constant initialization

struct S
{
    static const int c;
};

/*constexpr*/ int d = 10 * S::c; // not a constant expression: S::c has no preceding
                         // initializer, this initialization happens after const

const int S::c = 5;      // constant initialization, guaranteed to happen first

constexpr const int& return50()
{
  return S::c;
}


void constinit()
{
  std::cout << "d = " << d << '\n';
  std::array<int, S::c> a1; // OK: S::c is a constant expression
//  std::array<int, d> a2;  // error: d is not a constant expression
}


// --- zero initialization (since 98)

void zeroinit ()
{
  int a1;
  int a2();
  static int a3;
}


// --- value initalization (since 03)

struct S1
{
  int i;
};

struct S2
{
  int i;
  S2(){}
};


void valueinit ()
{
  std::cout << "int() = " << int() << std::endl;
  std::cout << "int{} = " << int{} << std::endl;
  std::cout << "S1().i = " << S1().i << std::endl;
  std::cout << "S2().i = " << S2().i << std::endl;
}

// --- list initialization (since 11)

template <typename T>
T passT (T t)
{
  return t;
}

struct S3
{
  int mem;
  std::string str{"default"};
  S3(int a, std::string s) : mem(a), str{s} {}
};

void listinit()
{
  std::initializer_list<int> li = {1,2,3,4,5};
  li = passT<std::initializer_list<int>>({3,4,5});
  for (auto x: li)
    std::cout << "x = " << x << std::endl;

  S3 s3 = passT<S3>({3, "sdafa"});
  std::cout << "s3.mem = " << s3.mem << std::endl;
  std::cout << "s3.str = " << s3.str.c_str() << std::endl;
  std::cout << "s3.str = " << S3({1, "test"}).str.c_str() << std::endl;
  // narrowing conversion
//  int bad{12.0};
//  unsigned char ch{-12};
}


// --- aggregate initialization (since 11)

struct AG
{
  int mem1;
  int mem2;
};

void aggreinit()
{
  // narrowing conversion
//  AG ag = {1, 2.0};
//  int a1[]{1, 2.0};
//  int a2[] = {1, 2.0};
}


void testInitalization()
{
  std::cout << "Initialization funcs" << std::endl;

  defaultinit();
  constinit();
  zeroinit();
  valueinit ();
  listinit();
}
