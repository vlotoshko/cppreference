#include "type_deduction.h"
#include <cstdlib>
#include <iostream>

// --- C++1y - type deduction

// using undefined template to findout type T by compiler error

// paramType is a reference
template<typename T>
void errFunc1(T& param);

// paramType is a universal reference
template<typename T>
void errFunc2(T&& param);

// paramType is nor a reference neither universal reference
template<typename T>
void errFunc3(T param);


template< typename T, std::size_t N >
constexpr std::size_t arraySize(T (&) [N]) {
    return N;
}

void someFunc(double, int) {}

// difference in auto and template type dedution - std::initializer_list

template<typename T>
void ilFunc(std::initializer_list<T> param);


// auto as trailing return type
template <typename Container, typename Index>
auto autoTRTFunc(Container& c, Index i) -> decltype(c[i]) {
    return c[i];
}

// auto uses template type deduction
template <typename Container, typename Index>
auto autoTTDlFunc(Container& c, Index i) {
    return c[i];
}

// decltype(auto) means type should be deducted but using decltype rules
template <typename Container, typename Index>
decltype(auto)
retDeclFunc(Container& c, Index i) {
    return c[i];
}



void testTypeDeduction() {
    int x = 27;
    const int cx = x;
    const int& rx = x;

    const char name[] = "array";

    int a[5];
//    errFunc2(a);

//    errFunc1(x);
//    errFunc1(cx);
//    errFunc1(rx);
//    errFunc1(42);
//    errFunc1(name);
//    errFunc1(someFunc);

//    errFunc2(x);
//    errFunc2(cx);
//    errFunc2(rx);
//    errFunc2(42);
//    errFunc2(name);
//    errFunc2(someFunc);

//    errFunc3(x);
//    errFunc3(cx);
//    errFunc3(rx);
//    errFunc3(42);
//    errFunc3(name);
//    errFunc3(someFunc);

    std::cout << arraySize(name) << std::endl;

//    auto a = {27};
//    errFunc3(a); // std::initalizer_list<int>
//    ilFunc({1,2,3});

    auto a1 = rx;
//    errFunc1(a1);
//    decltype(auto) a2 = rx;

}
