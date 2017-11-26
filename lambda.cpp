#include <iostream>
#include <vector>
#include <functional>
#include "lambda.hpp"

using lambda = std::function<void (int)>;

// TODO: function passing or capturing, timing function
// TODO: bind functions howto

class LambdaHolder
{
public:
    LambdaHolder(std::string& str) : _strValue(str) {}
    void addLambda(const lambda la)
    {
        _lambdas.push_back(la);
    }

    void invokeAll(int i) const
    {
        std::string initStr = "initial str";
        for(auto& la : _lambdas)
        {
            _strValue = initStr;
            la(i);
            if (_strValue != initStr)
            {
                std::cout << "--- changed after lambda: from '" << initStr.c_str() << "' to '" << _strValue.c_str() << "'"<< std::endl;
            }
        }
    }
private:
    std::vector<lambda> _lambdas;
    std::string& _strValue;
};

class TestThis
{
public:
    void addLambdaWithThisCapturing(LambdaHolder & lh) const
	{
        lh.addLambda([this](int x)
		{
            std::cout << "'this' capturing: " << _value + x << std::endl;
        });
	}
	
    void addLambdaWithThisCapturingImplicitly(LambdaHolder & lh) const
    {
        lh.addLambda([=](int x)
        {
            // _value implicitly used like this->_value
            std::cout << "'this' capturing implicitly: " << _value + x << std::endl;
        });
    }

    void addLambdaWithMemberClassCapturingByValue(LambdaHolder & lh) const
	{
		// member class capturing with init capture, not supported by C++11
        lh.addLambda([_value = _value](int x)
		{
            std::cout << "member class capturing: " << _value + x << std::endl;
        });
	}
	
    void addLambdaWithStaticMember(LambdaHolder & lh) const
	{
        lh.addLambda([=](int x)
		{
			// static members cannot be captured, so there is no capturing by value
            std::cout << "static members cannot be captured: "
            << TestThis::staticValue + x << std::endl;
        });
	}

	static int staticValue;
	void setValue(int v)
	{
		_value = v;
	}
private:
	int _value;
};

class ClassLikeClosure
{
public:
    ClassLikeClosure(std::string& str) : _strValue(str) {}
    void operator() (int) const
    {
        _strValue = "changed string 5";
        std::cout << "class like closure: " << _strValue.c_str() << std::endl;
    }
private:
    std::string& _strValue;
};

int TestThis::staticValue = 5;

void testLambda()
{
    std::string str;
    LambdaHolder lh(str);

    // Captures nothing
    lh.addLambda([](int)
    {
        std::cout << "captures_nothing" << std::endl;
    });

    // Captures by value
    lh.addLambda([str] (int) mutable
    {
        str = "changed string 1";
        std::cout << "captures_by_value: " << str.c_str() << std::endl;
    });

    // Captures all by value
    lh.addLambda([=] (int) mutable
    {
        str = "changed string 2";
        std::cout << "captures_all_by_value: " << str.c_str() << std::endl;
    });

    // Captures by reference
    lh.addLambda([&str] (int) mutable
    {
        str = "changed string 3";
        std::cout << "captures_by_reference: " << str.c_str() << std::endl;
    });

    // Captures all by reference
    lh.addLambda([&] (int) mutable
    {
        str = "changed string 4";
        std::cout << "captures_all_by_reference: " << str.c_str() << std::endl;
    });

    TestThis tt;
    tt.setValue(2);
    tt.addLambdaWithThisCapturing(lh);
    tt.addLambdaWithMemberClassCapturingByValue(lh);
    tt.addLambdaWithStaticMember(lh);

    {
        TestThis danglingT;
        danglingT.setValue(6);
        // dangling this, danglingT destroyed before used in the lambda
        danglingT.addLambdaWithThisCapturingImplicitly(lh);
    }

    ClassLikeClosure clc(str);
    lh.addLambda(clc);


    lh.addLambda([str = str] (int) mutable
    {
        str = "changed string 6";
        std::cout << "captures_all_by_reference: " << str.c_str() << std::endl;
    });

    // generalized lambda expression, since C++14
    lh.addLambda([str = str] (auto) mutable
    {
        str = "changed string 7";
        std::cout << "captures_all_by_reference: " << str.c_str() << std::endl;
    });

    lh.invokeAll(1);

}