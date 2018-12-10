#include <iostream>
#include <string>
#include <locale>
#include "strings.hpp"

using std::string;
using std::wstring;

void testString()
{
    wchar_t wc[] = L"Тексt на кирилиці";
    char     c[] =  "Тексt на кирилиці";
    std::wcout << wc << std::endl;
    std::cout << c << std::endl;

    wstring s = L"Marry had a very little lamp.";
    wstring s1 = L"Joe" + s.substr(s.find(L" "), string::npos);
    std::wcout << s1 << std::endl;

    std::cout << "s.size = " << s.size() << std::endl;
    std::cout << "s.length = " << s.length() << std::endl;
    std::cout << "s.capacity = " << s.capacity() << std::endl;
    s.reserve(100);
    std::cout << "s.capacity = " << s.capacity() << std::endl;

    const wchar_t e[] = L"\x0456";
    std::wcout << e << std::endl;
    std::wcout << *e << std::endl;
}
