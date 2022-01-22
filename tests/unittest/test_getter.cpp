#include "test.h"
#include "accessorpp/getter.h"

#include <iostream>

struct MyValue
{
	explicit MyValue(const int value = 0)
		: value(value) {
	}

	int get() const {
		return value;
	}

	int value;
};

TEST_CASE("Getter, int, variable")
{
	int value = 5;
	accessorpp::Getter<int &> getter(&value);
	REQUIRE(getter.get() == 5);
	value = 8;
	REQUIRE(getter.get() == 8);
}

TEST_CASE("Getter, const int &, variable")
{
	int value = 3;
	const int & ref = value;
	accessorpp::Getter<const int &> getter(&ref);
	REQUIRE(getter.get() == 3);
	value = 6;
	REQUIRE(getter.get() == 6);
}

TEST_CASE("Getter, int &, variable")
{
	int value = 38;
	int & ref = value;
	accessorpp::Getter<int &> getter(&ref);
	REQUIRE(getter.get() == 38);
	value = 98;
	REQUIRE(getter.get() == 98);
}

TEST_CASE("Getter, int, member")
{
	MyValue myValue(9);
	accessorpp::Getter<int> getter(&MyValue::value, &myValue);
	REQUIRE(getter.get() == 9);
	myValue.value = 8;
	REQUIRE(getter.get() == 8);
}

TEST_CASE("Getter, const int &, member")
{
	MyValue myValue(9);
	accessorpp::Getter<const int &> getter(&MyValue::value, &myValue);
	REQUIRE(getter.get() == 9);
	myValue.value = 8;
	REQUIRE(getter.get() == 8);
}

TEST_CASE("Getter, int &, member")
{
	MyValue myValue(9);
	accessorpp::Getter<int &> getter(&MyValue::value, &myValue);
	REQUIRE(getter.get() == 9);
	myValue.value = 8;
	REQUIRE(getter.get() == 8);
}

TEST_CASE("test getter")
{
	int value = 5;
	accessorpp::Getter<int &> getter1(&value);
	REQUIRE(getter1.get() == 5);

	accessorpp::Getter<const int &> getter2(8);
	REQUIRE(getter2 == 8);

	accessorpp::Getter<int> getter3([]() { return 6; });
	REQUIRE(getter3 == 6);

	MyValue myValue(9);
	accessorpp::Getter<int> getter4(&MyValue::get, &myValue);
	REQUIRE(getter4 == 9);

	accessorpp::Getter<int> getter5(&MyValue::value, &myValue);
	REQUIRE(getter5 == 9);

	std::cout << getter5 << std::endl;
}

