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
#include "accessorpp/accessor.h"

struct MyValue
{
	MyValue(const int value = 0)
		: value(value) {
	}

	int getValue() const {
		return value;
	}

	const int & getCref() const {
		return value;
	}

	int & getRef() {
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

TEST_CASE("Accessor, default storage, defaultGetter, setter")
{
	using Accessor = accessorpp::Accessor<int>;

	{
		Accessor * ptr;
		Accessor accessor(accessorpp::defaultGetter, [&ptr](const int newValue) {
			ptr->directSet(newValue * 2);
		});
		ptr = &accessor;
		REQUIRE(accessor == 0);
		REQUIRE(! accessor.isReadOnly());
		accessor = 5;
		REQUIRE(accessor == 10);
	}
}

TEST_CASE("Accessor, default storage, getter, defaultSetter")
{
	using Accessor = accessorpp::Accessor<int>;

	{
		// accessor always returns 5
		Accessor accessor(5, accessorpp::defaultSetter, 3);
		REQUIRE(accessor == 5);
		REQUIRE(! accessor.isReadOnly());
		accessor = 8; // no effect
		REQUIRE(accessor == 5);
	}
}

TEST_CASE("Accessor, default storage, defaultGetter, noSetter")
{
	using Accessor = accessorpp::Accessor<int>;

	{
		Accessor accessor(accessorpp::defaultGetter, accessorpp::noSetter);
		REQUIRE(accessor == 0);
		REQUIRE(accessor.isReadOnly());
		CHECK_THROWS(accessor = 5);
	}

	{
		Accessor accessor(accessorpp::defaultGetter, accessorpp::noSetter, 3);
		REQUIRE(accessor == 3);
		REQUIRE(accessor.isReadOnly());
		CHECK_THROWS(accessor = 5);
	}
}

TEST_CASE("Accessor, default storage, getter, noSetter")
{
	using Accessor = accessorpp::Accessor<int>;

	{
		int n = 5;
		Accessor accessor(&n, accessorpp::noSetter, 3);
		REQUIRE(accessor == 5);
		REQUIRE(accessor.isReadOnly());
		CHECK_THROWS(accessor = 6);
	}

	{
		MyValue myValue(8);
		Accessor accessor(Accessor::GetterType(&MyValue::getValue, &myValue), accessorpp::noSetter);
		REQUIRE(accessor == 8);
		REQUIRE(accessor.isReadOnly());
		CHECK_THROWS(accessor = 6);
	}
}

TEST_CASE("Accessor, default storage, defaultGetter, defaultSetter")
{
	using Accessor = accessorpp::Accessor<int>;

	{
		Accessor accessor(accessorpp::defaultGetter, accessorpp::defaultSetter, 3);
		REQUIRE(accessor == 3);
		REQUIRE(! accessor.isReadOnly());
		accessor = 5;
		REQUIRE(accessor == 5);
	}
}
