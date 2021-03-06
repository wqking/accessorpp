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
		return accessor.directGet();
	});
	accessor.setSetter([&accessor, &setCount](const int value) {
		++setCount;
		accessor.directSet(value);
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

struct NoStoragePolicies
{
	using Storage = accessorpp::ExternalStorage;
};

TEST_CASE("Accessor, int, ExternalStorage, variable")
{
	int value{};
	SECTION("Use ctor") {
		accessorpp::Accessor<int, NoStoragePolicies> accessor(&value, &value);
		REQUIRE(! accessorpp::Accessor<int, NoStoragePolicies>::internalStorage);

		REQUIRE(accessor.get() == 0);

		accessor = 3;
		REQUIRE(accessor.get() == 3);

		accessor = 8;
		REQUIRE(accessor.get() == 8);
	}

	SECTION("Use createAccessor<int>") {
		auto accessor(accessorpp::createAccessor<int>(&value, &value, NoStoragePolicies()));
		REQUIRE(! accessorpp::Accessor<int, NoStoragePolicies>::internalStorage);

		REQUIRE(accessor.get() == 0);

		accessor = 3;
		REQUIRE(accessor.get() == 3);

		accessor = 8;
		REQUIRE(accessor.get() == 8);
	}

	SECTION("Use createAccessor") {
		auto accessor(accessorpp::createAccessor(&value, &value, NoStoragePolicies()));
		REQUIRE(! accessorpp::Accessor<int, NoStoragePolicies>::internalStorage);

		REQUIRE(accessor.get() == 0);

		accessor = 3;
		REQUIRE(accessor.get() == 3);

		accessor = 8;
		REQUIRE(accessor.get() == 8);
	}
}

TEST_CASE("Accessor, int, ExternalStorage, member, embed instance")
{
	MyValue myValue;
	SECTION("Use ctor") {
		accessorpp::Accessor<int, NoStoragePolicies> accessor(&MyValue::value, &myValue, &MyValue::value, &myValue);
		REQUIRE(accessor.get() == 0);

		accessor = 3;
		REQUIRE(accessor.get() == 3);

		accessor = 8;
		REQUIRE(accessor.get() == 8);
	}

	SECTION("Use createAccessor<int>") {
		auto accessor(accessorpp::createAccessor<int>(&MyValue::value, &myValue, &MyValue::value, &myValue, NoStoragePolicies()));
		REQUIRE(accessor.get() == 0);

		accessor = 3;
		REQUIRE(accessor.get() == 3);

		accessor = 8;
		REQUIRE(accessor.get() == 8);
	}

	SECTION("Use createAccessor") {
		auto accessor(accessorpp::createAccessor(&MyValue::value, &myValue, &MyValue::value, &myValue, NoStoragePolicies()));
		REQUIRE(accessor.get() == 0);

		accessor = 3;
		REQUIRE(accessor.get() == 3);

		accessor = 8;
		REQUIRE(accessor.get() == 8);
	}
}

TEST_CASE("Accessor, int, ExternalStorage, member, pass instance")
{
	MyValue myValue;
	accessorpp::Accessor<int, NoStoragePolicies> accessor(&MyValue::value, &MyValue::value);
	REQUIRE(accessor.get(&myValue) == 0);

	accessor.set(3, &myValue);
	REQUIRE(accessor.get(&myValue) == 3);

	accessor.set(8, &myValue);
	REQUIRE(accessor.get(&myValue) == 8);
}

TEST_CASE("Accessor, int, ExternalStorage, member getValue() setValue(), embed instance")
{
	MyValue myValue;
	accessorpp::Accessor<int, NoStoragePolicies> accessor(&MyValue::getValue, &myValue, &MyValue::setValue, &myValue);
	REQUIRE(accessor.get() == 0);

	accessor = 3;
	REQUIRE(accessor.get() == 3);

	accessor = 8;
	REQUIRE(accessor.get() == 8);
}

TEST_CASE("Accessor, int, ExternalStorage, member getValue() setValue(), pass instance")
{
	MyValue myValue;
	accessorpp::Accessor<int, NoStoragePolicies> accessor(&MyValue::getValue, &MyValue::setValue);
	REQUIRE(accessor.get(&myValue) == 0);

	accessor.set(3, &myValue);
	REQUIRE(accessor.get(&myValue) == 3);

	accessor.set(8, &myValue);
	REQUIRE(accessor.get(&myValue) == 8);
}

TEST_CASE("Accessor, default storage, read only")
{
	using Accessor = accessorpp::Accessor<int>;

	{
		Accessor accessor(accessorpp::defaultGetter, accessorpp::noSetter);
		REQUIRE(accessor == 0);
		REQUIRE(accessor.isReadOnly());
		CHECK_THROWS(accessor = 5);
	}

	{
		auto accessor(accessorpp::createReadOnlyAccessor<int>(accessorpp::defaultGetter));
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

	{
		int n = 5;
		Accessor accessor(&n, accessorpp::noSetter, 3);
		REQUIRE(accessor == 5);
		REQUIRE(accessor.isReadOnly());
		CHECK_THROWS(accessor = 6);
	}

	{
		Accessor accessor(accessorpp::defaultGetter, accessorpp::defaultSetter, 3);
		REQUIRE(accessor == 3);
		REQUIRE(! accessor.isReadOnly());
		accessor = 5;
		REQUIRE(accessor == 5);
	}
}

TEST_CASE("This is the play camp for sample code in the document")
{
	struct Policies {
		using OnChangingCallback = std::function<void (int)>;
		using OnChangedCallback = std::function<void ()>;
	};

	using AccessorType = accessorpp::Accessor<
		int,
		Policies
	>;
	AccessorType accessor;
	accessor.onChanging() = [&accessor](const int newValue) {
		std::cout << "onChanging: new value = " << newValue << " old value = " << accessor << std::endl;
	};
	accessor.onChanged() = [&accessor]() {
		std::cout << "onChanged: new value = " << accessor << std::endl;
	};

	// output
	// onChanging: new value = 5 old value = 0
	// onChanged: new value = 5
	accessor = 5;

	// output
	// onChanging: new value = 38 old value = 5
	// onChanged: new value = 38
	accessor = 38;
}
