#include <iostream>
#include "lambda.hpp"

class TestThis
{
public:
	void lambdaInvoke(int a) const
	{
		auto la = [this](int x)
		{
			std::cout << "test 'this' passing: " << _value + x << std::endl;
		};

		la(a);
	}

	void setValue(int v)
	{
		_value = v;
	}
private:
	int _value;
};

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
	t.lambdaInvoke(2);
}
