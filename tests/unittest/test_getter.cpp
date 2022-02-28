// accessorpp library
// Copyright (C) 2022 Wang Qi (wqking)
// Github: https://github.com/wqking/accessorpp
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "test.h"
#include "accessorpp/getter.h"

#include <sstream>

TEST_CASE("Getter, ctor")
{
	int value = 5;
	accessorpp::Getter<int> getter1(&value);
	REQUIRE(getter1.get() == 5);

	accessorpp::Getter<int> getter2(getter1);
	REQUIRE(getter2.get() == 5);
}

TEST_CASE("Getter, int, variable")
{
	int value = 5;
	accessorpp::Getter<int> getter(&value);
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

struct MyValue
{
	explicit MyValue(const int value = 0)
		: value(value) {
	}

	int getValue() const {
		return value;
	}

	void set(const int newValue) const {
		const_cast<MyValue *>(this)->value = newValue;
	}

	const int & getCref() const {
		return value;
	}

	int & getRef() {
		return value;
	}

	operator int() const {
		return value;
	}

	int value;
};

TEST_CASE("Getter, int, member")
{
	MyValue myValue(9);

	SECTION("Embed instance")
	{
		accessorpp::Getter<int> getter(&MyValue::value, &myValue);
		REQUIRE(getter.get() == 9);
		myValue.value = 8;
		REQUIRE(getter.get() == 8);
	}

	SECTION("Pass instance")
	{
		accessorpp::Getter<int> getter(&MyValue::value);
		REQUIRE(getter.get(&myValue) == 9);
		myValue.value = 8;
		REQUIRE(getter.get(&myValue) == 8);
	}
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

TEST_CASE("Getter, int, member getValue()")
{
	MyValue myValue(9);
	
	SECTION("Embed instance")
	{
		accessorpp::Getter<int> getter(&MyValue::getValue, &myValue);
		REQUIRE(getter.get() == 9);
		myValue.value = 8;
		REQUIRE(getter.get() == 8);
	}
	
	SECTION("Pass instance")
	{
		accessorpp::Getter<int> getter(&MyValue::getValue);
		REQUIRE(getter.get(&myValue) == 9);
		myValue.value = 8;
		REQUIRE(getter.get(&myValue) == 8);
	}
}

TEST_CASE("Getter, int, member getCref()")
{
	MyValue myValue(9);
	accessorpp::Getter<int> getter(&MyValue::getCref, &myValue);
	REQUIRE(getter.get() == 9);
	myValue.value = 8;
	REQUIRE(getter.get() == 8);
}

TEST_CASE("Getter, const int &, member getCref()")
{
	{
		MyValue myValue(9);
		accessorpp::Getter<const int &> getter(&MyValue::getCref, &myValue);
		REQUIRE(getter.get() == 9);
		myValue.value = 8;
		REQUIRE(getter.get() == 8);
	}
	{
		const MyValue myValue(9);
		accessorpp::Getter<const int &> getter(&MyValue::getCref, &myValue);
		REQUIRE(getter.get() == 9);
		myValue.set(8);
		REQUIRE(getter.get() == 8);
	}
}

TEST_CASE("Getter, int, member getRef()")
{
	MyValue myValue(9);
	accessorpp::Getter<int> getter(&MyValue::getRef, &myValue);
	REQUIRE(getter.get() == 9);
	myValue.value = 8;
	REQUIRE(getter.get() == 8);
}

TEST_CASE("Getter, int, MyValue variable")
{
	MyValue myValue(9);
	accessorpp::Getter<MyValue> getter(&myValue);
	REQUIRE(getter.get().getValue() == 9);
	myValue.value = 8;
	REQUIRE(getter.get().getValue() == 8);
	myValue.value = 3;
	const int converted = (MyValue)getter;
	REQUIRE(converted == 3);
}

TEST_CASE("Getter, ostream")
{
	int value = 5;
	accessorpp::Getter<int> getter(&value);
	std::stringstream stream1;
	std::stringstream stream2;
	stream1 << getter;
	REQUIRE(stream1.str() != stream2.str());
	stream2 << 5;
	REQUIRE(stream1.str() == stream2.str());
}
