#include <iostream>
#include "lambda.hpp"

void testLambda()
{
    std::cout << "test Lambda stub" << std::endl;
    auto la = [](int i)
    {
    	return ++i;
    };
//    int i = la;
}