#include "other.h"

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


void testOther(){
//   isLucky(true);
}
