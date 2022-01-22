#include "test.h"
#include "accessorpp/setter.h"

#include <iostream>

struct MyValue
{
	explicit MyValue(const int value = 0) : value(value) {
	}

	void set(const int v) {
		value = v;
	}

	int value;
};

TEST_CASE("test setter")
{
	int value = 0;
	accessorpp::Setter<int &> setter1(&value);
	int value2 = 5;
	setter1.set(value2);
	REQUIRE(value == 5);

	accessorpp::Setter<int> setter2([&value](const int v){ value = v; });
	setter2.set(8);
	REQUIRE(value == 8);

	MyValue myValue(9);
	accessorpp::Setter<int> setter3(&MyValue::set, &myValue);
	REQUIRE(myValue.value != 8);
	setter3 = (char)8;
	REQUIRE(myValue.value == 8);
	
	accessorpp::Setter<int> setter4(&MyValue::value, &myValue);
	REQUIRE(myValue.value != 5);
	setter4 = (char)5;
	REQUIRE(myValue.value == 5);
}
