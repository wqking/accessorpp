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

#include <iostream>
#include <string>

TEST_CASE("Accessor, ctor")
{
	accessorpp::Accessor<int> accessor1;
	REQUIRE(accessor1.get() == 0);

	accessor1 = 3;
	REQUIRE(accessor1.get() == 3);

	accessorpp::Accessor<int> accessor2(accessor1);
	REQUIRE(accessor2.get() == 3);
	accessor2 = 5;
	REQUIRE(accessor1.get() == 3);
	REQUIRE(accessor2.get() == 5);
}

TEST_CASE("Accessor, int *, default storage, variable")
{
	accessorpp::Accessor<int *> accessor;
	REQUIRE(accessor.get() == nullptr);

	int value1;
	accessor = &value1;
	REQUIRE(accessor.get() == &value1);

	int value2;
	accessor = &value2;
	REQUIRE(accessor.get() == &value2);
}

TEST_CASE("Accessor, void *, default storage, variable")
{
	accessorpp::Accessor<void *> accessor;
	REQUIRE(accessor.get() == nullptr);

	int value1;
	accessor = &value1;
	REQUIRE(accessor.get() == &value1);

	int value2;
	accessor = &value2;
	REQUIRE(accessor.get() == &value2);
}

TEST_CASE("Accessor, int, default storage, variable")
{
	accessorpp::Accessor<int> accessor;
	REQUIRE(accessor.get() == 0);

	accessor = 3;
	REQUIRE(accessor.get() == 3);

	accessor = 8;
	REQUIRE(accessor.get() == 8);
}

TEST_CASE("Accessor, const int &, default storage, variable")
{
	accessorpp::Accessor<const int &> accessor;
	REQUIRE(accessor.get() == 0);

	accessor = 3;
	REQUIRE(accessor.get() == 3);

	accessor = 8;
	REQUIRE(accessor.get() == 8);
}

TEST_CASE("Accessor, int &, default storage, variable")
{
	accessorpp::Accessor<int &> accessor;
	REQUIRE(accessor.get() == 0);

	int value = 3;
	accessor = value;
	REQUIRE(accessor.get() == 3);

	value = 8;
	accessor = value;
	REQUIRE(accessor.get() == 8);
}

TEST_CASE("Accessor, bool, default storage, variable")
{
	accessorpp::Accessor<bool> accessor;
	REQUIRE(accessor.get() == false);

	accessor = true;
	REQUIRE(accessor.get() == true);

	accessor = false;
	REQUIRE(accessor.get() == false);
}

TEST_CASE("Accessor, std::string, default storage, variable")
{
	accessorpp::Accessor<std::string> accessor;
	REQUIRE(accessor.get() == "");

	accessor = "Hello";
	REQUIRE(accessor.get() == "Hello");

	accessor = "World";
	REQUIRE(accessor == "World");
}

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

TEST_CASE("Accessor, MyValue, default storage")
{
	accessorpp::Accessor<MyValue> accessor;
	REQUIRE(accessor.get().getValue() == 0);

	accessor = MyValue(3);
	REQUIRE(accessor.get().getValue() == 3);

	accessor = MyValue(8);
	REQUIRE(accessor.get().getValue() == 8);
}

TEST_CASE("Accessor, int, default storage, variable, customized getter/setter")
{
	int getCount = 0;
	int setCount = 0;
	accessorpp::Accessor<int> accessor;
	accessor.setGetter([&accessor, &getCount]() {
		++getCount;
		return accessor.getValue();
	});
	accessor.setSetter([&accessor, &setCount](const int value) {
		++setCount;
		accessor.setValue(value);
	});

	REQUIRE(accessor.get() == 0);
	REQUIRE(getCount == 1);
	REQUIRE(setCount == 0);

	accessor = 3;
	REQUIRE(getCount == 1);
	REQUIRE(setCount == 1);
	REQUIRE(accessor.get() == 3);
	REQUIRE(getCount == 2);
	REQUIRE(setCount == 1);

	accessor = 8;
	REQUIRE(getCount == 2);
	REQUIRE(setCount == 2);
	REQUIRE(accessor.get() == 8);
	REQUIRE(getCount == 3);
	REQUIRE(setCount == 2);
}

TEST_CASE("Accessor, int, NoStorage, variable")
{
	int value{};
	accessorpp::Accessor<int, accessorpp::NoStorage> accessor(&value);
	REQUIRE(accessor.get() == 0);

	accessor = 3;
	REQUIRE(accessor.get() == 3);

	accessor = 8;
	REQUIRE(accessor.get() == 8);
}

TEST_CASE("Accessor, int, NoStorage, member")
{
	MyValue myValue;
	accessorpp::Accessor<int, accessorpp::NoStorage> accessor(&MyValue::value, &myValue);
	REQUIRE(accessor.get() == 0);

	accessor = 3;
	REQUIRE(accessor.get() == 3);

	accessor = 8;
	REQUIRE(accessor.get() == 8);
}

TEST_CASE("Accessor, int, NoStorage, member getValue() setValue()")
{
	MyValue myValue;
	accessorpp::Accessor<int, accessorpp::NoStorage> accessor(&MyValue::getValue, &myValue, &MyValue::setValue, &myValue);
	REQUIRE(accessor.get() == 0);

	accessor = 3;
	REQUIRE(accessor.get() == 3);

	accessor = 8;
	REQUIRE(accessor.get() == 8);
}

TEST_CASE("Accessor, callback")
{
	using AccessorType = accessorpp::Accessor<
		const int &,
		accessorpp::UseStorage,
		std::function<void (int, int)>
	>;
	int newValue = 0;
	int oldValue = 0;
	AccessorType accessor;
	accessor.getCallback() = [&newValue, &oldValue](const int newValue_, const int oldValue_) {
		newValue = newValue_;
		oldValue = oldValue_;
	};
	REQUIRE(newValue == 0);
	REQUIRE(oldValue == 0);

	accessor = 3;
	REQUIRE(newValue == 3);
	REQUIRE(oldValue == 0);

	accessor = 8;
	REQUIRE(newValue == 8);
	REQUIRE(oldValue == 3);
}
