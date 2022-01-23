#include "test.h"
#include "accessorpp/setter.h"

#include <sstream>

TEST_CASE("Setter, ctor")
{
	int value = 0;
	accessorpp::Setter<int> setter1(&value);
	setter1 = 8;
	REQUIRE(value == 8);

	accessorpp::Setter<int> setter2(setter1);
	setter2 = 5;
	REQUIRE(value == 5);
}

TEST_CASE("Setter, int, variable")
{
	int value = 0;
	accessorpp::Setter<int> setter(&value);
	REQUIRE(value == 0);
	setter = 8;
	REQUIRE(value == 8);
}

TEST_CASE("Setter, const int &, variable")
{
	int value = 0;
	accessorpp::Setter<const int &> setter(&value);
	REQUIRE(value == 0);
	setter = 8;
	REQUIRE(value == 8);
}

TEST_CASE("Setter, int &, variable")
{
	int value = 0;
	accessorpp::Setter<int &> setter(&value);
	REQUIRE(value == 0);
	int assign = 8;
	setter = assign;
	REQUIRE(value == 8);
}

struct MyValue
{
	MyValue(const int value = 0)
		: value(value) {
	}

	int getValue() const {
		return value;
	}

	void setValue(const int newValue) {
		value = newValue;
	}

	void setConst(const int newValue) const {
		const_cast<MyValue *>(this)->value = newValue;
	}

	int value;
};

TEST_CASE("Setter, int, member")
{
	MyValue myValue(0);
	REQUIRE(myValue.getValue() == 0);
	accessorpp::Setter<int> setter(&MyValue::value, &myValue);
	setter = 8;
	REQUIRE(myValue.getValue() == 8);
}

TEST_CASE("Setter, const int &, member")
{
	MyValue myValue(0);
	REQUIRE(myValue.getValue() == 0);
	accessorpp::Setter<const int &> setter(&MyValue::value, &myValue);
	setter = 8;
	REQUIRE(myValue.getValue() == 8);
	const int assign = 38;
	setter = assign;
	REQUIRE(myValue.getValue() == 38);
}

TEST_CASE("Setter, int &, member")
{
	MyValue myValue(0);
	REQUIRE(myValue.getValue() == 0);
	accessorpp::Setter<int &> setter(&MyValue::value, &myValue);
	int assign = 8;
	setter = assign;
	REQUIRE(myValue.getValue() == 8);
}

TEST_CASE("Setter, int, member setValue")
{
	MyValue myValue(0);
	REQUIRE(myValue.getValue() == 0);
	accessorpp::Setter<int> setter(&MyValue::setValue, &myValue);
	setter = 8;
	REQUIRE(myValue.getValue() == 8);
}

TEST_CASE("Setter, const int &, member setValue")
{
	MyValue myValue(0);
	REQUIRE(myValue.getValue() == 0);
	accessorpp::Setter<const int &> setter(&MyValue::setValue, &myValue);
	setter = 8;
	REQUIRE(myValue.getValue() == 8);
}

TEST_CASE("Setter, int &, member setValue")
{
	MyValue myValue(0);
	REQUIRE(myValue.getValue() == 0);
	accessorpp::Setter<int &> setter(&MyValue::setValue, &myValue);
	int assign = 8;
	setter = assign;
	REQUIRE(myValue.getValue() == 8);
}

TEST_CASE("Setter, int, member setConst")
{
	MyValue myValue(0);
	REQUIRE(myValue.getValue() == 0);
	accessorpp::Setter<int> setter(&MyValue::setConst, &myValue);
	setter = 8;
	REQUIRE(myValue.getValue() == 8);
}

TEST_CASE("Setter, const int &, member setConst")
{
	MyValue myValue(0);
	REQUIRE(myValue.getValue() == 0);
	accessorpp::Setter<const int &> setter(&MyValue::setConst, &myValue);
	setter = 8;
	REQUIRE(myValue.getValue() == 8);
}

TEST_CASE("Setter, int &, member setConst")
{
	MyValue myValue(0);
	REQUIRE(myValue.getValue() == 0);
	accessorpp::Setter<int &> setter(&MyValue::setConst, &myValue);
	int assign = 8;
	setter = assign;
	REQUIRE(myValue.getValue() == 8);
}

TEST_CASE("Setter, istream")
{
	int value = 5;
	accessorpp::Setter<int> setter(&value);
	std::stringstream stream;
	REQUIRE(value == 5);
	stream.str("38");
	stream >> setter;
	REQUIRE(value == 38);
}
