#include <iostream>
#include "initialization.h"
#include "strings.h"
#include "nvi_and_crtp.h"
#include "type_deduction.h"
#include "smart_pointers.h"
#include "other.h"
#include <array>

using std::cout;
using std::endl;


int passRValue(int&& i) {
    cout << i++ << endl;
    cout << i << endl;

    return 0;
}


void testRValue(){
    passRValue(3 + 2);
    passRValue(3);
    int i = 1;
    passRValue(std::move(i));

    int a = 9;
    int& la = a;
    int&& ra = passRValue(7);

}



class CConnectionTest
{
public:
    CConnectionTest()
    {

    }
    virtual ~CConnectionTest(){}

    std::string getResult()
    {
        std::array<char, CMode::Last> result;
        int idx = 0;
        for (auto & r : result)
        {
           r = toChar(m_Result[idx++]);
        }
        return std::string(result.data());
    }

protected:
    enum CResult { NotTested, Passed, Failed, Unknown };
    enum CMode { RemoteResource = 0, LocalResource, Login, Read, Write, Execute, Last };

    virtual CResult checkRemoteResource() { return NotTested; }
    virtual CResult checkLocalResource() { return NotTested; }
    virtual CResult checkLogin() { return NotTested; }
    virtual CResult checkCanRead() { return NotTested; }
    virtual CResult checkCanWrite() { return NotTested; }
    virtual CResult checkCanExecute() { return NotTested; }

    std::array<CResult, Last> m_Result = {{NotTested}};
private:
    char toChar(CResult res)
    {
        switch (res) {
        case NotTested:
            return '-';
            break;
        case Passed:
            return 'Y';
            break;
        case Failed:
            return 'N';
            break;
        default:
            return 'X';
            break;
        }
    }
};

class CLocalTest : public CConnectionTest
{
public:
    CLocalTest()
    {

    }
    void check()
    {
        if (checkLocalResource() == Passed)
        {
            std::cout << "Hello World!" << std::endl;
            checkCanRead();
            checkCanWrite();
            checkCanExecute();
        }
    }

protected:
    CResult checkLocalResource() override
    {
        if (m_Result[LocalResource] == NotTested)
        {
            // try to connect to local resource
            m_Result[LocalResource] = Passed;

            if (m_Result[LocalResource] != Passed)
            {
                m_Result[Read] = Unknown;
                m_Result[Write] = Unknown;
                m_Result[Execute] = Unknown;
            }
            std::cout << "Hello World! 1" << std::endl;
        }
        std::cout << "Hello World! 2" << std::endl;
        return m_Result[LocalResource];
    }

    CResult checkCanRead() override
    {
        if (m_Result[Read] == NotTested)
        {
            // try to connect to local resource
            m_Result[Read] = Passed;
        }
        return m_Result[Read];
    }

    CResult checkCanWrite() override
    {
        if (m_Result[Write] == NotTested)
        {
            // try to connect to local resource
            m_Result[Write] = Unknown;
        }
        return m_Result[Write];
    }

    CResult checkCanExecute() override
    {
        if (m_Result[Execute] == NotTested)
        {
            // try to connect to local resource
            m_Result[Execute] = Failed;
        }
        return m_Result[Execute];
    }
};


void testConnTest()
{
    CLocalTest c;
    c.check();
    std::cout << "result is: " << c.getResult() << std::endl;
//    if (c.m_Result[CLocalTest::CMode::LocalResource] == CLocalTest::CResult::NotTested)

}

int main(int argc, char **argv)
{
  std::cout << "Hello World!" << std::endl;
//  testOther();
//  testInitalization(argc, argv);
//  testNVI();
//  testCRTP();
//  testTypeDeduction();
//  testSmartPointers();

  testConnTest();
  testRValue();
  return 0;
}
