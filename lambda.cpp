#include <iostream>
#include <vector>
#include "lambda.hpp"

using lambda = std::function<void (int)>;

class LambdaHolder
{
private:
std::vector<lambda> lambdas;
public:
	void addLambda(const lambda la)
	{
		lambdas.push_back(la);
	}
	void invokeAll(int i) const
	{
		for( auto& la : lambdas)
		{
			la(i);
		}
	}
};

class TestThis
{
public:
	void lambdaWithThisCapturing(int a) const
	{
		auto la = [this](int x)
		{
			std::cout << "test 'this' passing: " << _value + x << std::endl;
		};
		la(a);
	}
	
	void lambdaWithThisCapturingImplicitly(int a) const
	{
		auto la = [=](int x)
		{
			// _value implicitly used like this->_value
			std::cout << "test 'this' passing: " << _value + x << std::endl;
		};
		la(a);
	}
	
	void lambdaWithMemberClassCapturingByValue(int a) const
	{
		auto la = [_value = _value](int x)
		{
			std::cout << "test 'this' passing: " << _value + x << std::endl;
		};
		la(a);
	}
	
	void lambdaWithStaticMember(int a) const
	{
		auto la = [=](int x)
		{
			// static members cannot be captured, so there is no capturing by value
			std::cout << "test 'this' passing: " << TestThis::staticValue + x << std::endl;
		};
		la(a);
	}
	
	static int staticValue;
	void setValue(int v)
	{
		_value = v;
	}
private:
	int _value;
};

int TestThis::staticValue = 5;

void testLambda()
{
	// Captures nothing
	auto captures_nothing = [](int i)
    {
    	return ++i;
	};
	std::cout << "captures_nothing: " <<  captures_nothing(6) <<  std::endl;

	std::string str{"some string"};

	// Captures by value
	auto captures_by_value = [str] (int i) mutable
	{
		str = "changed string 1";
		std::cout << "captures_by_value: " << str.c_str() << std::endl;
		return ++i;
	};

	captures_by_value(7);
	std::cout << "string now: " << str.c_str() << std::endl;


	// Captures all by value
	auto captures_all_by_value = [=] (int i) mutable
	{
		str = "changed string 2";
		std::cout << "captures_all_by_value: " << str.c_str() << std::endl;
		return ++i;
	};

	captures_all_by_value(7);
	std::cout << "string now: " << str.c_str() << std::endl;


	// Captures by reference
	auto captures_by_ref = [&str] (int i) mutable
	{
		str = "changed string 3";
		std::cout << "captures_by_reference: " << str.c_str() << std::endl;
		return ++i;
	};

	captures_by_ref(7);
	std::cout << "string now: " << str.c_str() << std::endl;


	// Captures all by reference
	auto captures_all_by_ref = [&] (int i) mutable
	{
		str = "changed string 4";
		std::cout << "captures_all_by_reference: " << str.c_str() << std::endl;
		return ++i;
	};

	captures_all_by_ref(7);
	std::cout << "string now: " << str.c_str() << std::endl;

	TestThis t;
	t.setValue(3);
	t.lambdaWithThisCapturing(2);
}