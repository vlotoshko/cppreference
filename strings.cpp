#include <iostream>
#include <string>
//#include <locale>
#include "strings.h"

using std::string;
using std::wstring;

stringsTest::stringsTest()
{
    wchar_t wc[] = L"Тексt на кирилиці";
    char     c[] =  "Тексt на кирилиці";
    wstring s = L"Marry had a very little lamp.";
//    string s1 = "Joe" + s.substr(s.find(" "), string::npos);

    std::wcout << wc << std::endl;
//    std::wcout << *wc << std::endl;

//    std::cout << c << std::endl;
//    std::cout << *c << std::endl;


    std::cout << "s.size = " << s.size() << std::endl;
    std::cout << "s.length = " << s.length() << std::endl;
    std::cout << "s.capacity = " << s.capacity() << std::endl;
    s.reserve(100);
    std::cout << "s.capacity = " << s.capacity() << std::endl;

    const wchar_t e[] = L"\x0456";
    std::wcout << e << std::endl;
    std::wcout << *e << std::endl;
}
