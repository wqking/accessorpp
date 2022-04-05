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

TEST_CASE("Accessor, callback")
{
	struct Policies {
		using OnChangingCallback = std::function<void (int)>;
		using OnChangedCallback = std::function<void ()>;
	};

	using AccessorType = accessorpp::Accessor<
		const int &,
		Policies
	>;
	struct ValuePair
	{
		int newValue;
		int oldValue;
	};
	ValuePair changingValue {};
	ValuePair changedValue {};
	AccessorType accessor;
	accessor.onChanging() = [&changingValue, &accessor](const int newValue) {
		changingValue.newValue = newValue;
		changingValue.oldValue = accessor;
	};
	accessor.onChanged() = [&changedValue, &accessor]() {
		changedValue.newValue = accessor;
	};
	REQUIRE(changingValue.newValue == 0);
	REQUIRE(changingValue.oldValue == 0);
	REQUIRE(changedValue.newValue == 0);
	REQUIRE(changedValue.oldValue == 0);

	accessor = 3;
	REQUIRE(changingValue.newValue == 3);
	REQUIRE(changingValue.oldValue == 0);
	REQUIRE(changedValue.newValue == 3);
	REQUIRE(changedValue.oldValue == 0);

	accessor = 8;
	REQUIRE(changingValue.newValue == 8);
	REQUIRE(changingValue.oldValue == 3);
	REQUIRE(changedValue.newValue == 8);
	REQUIRE(changedValue.oldValue == 0);
}

TEST_CASE("Accessor, callback, CallbackData")
{
	struct Policies {
		using OnChangingCallback = std::function<void (int, std::string)>;
		using OnChangedCallback = std::function<void ()>;
		using CallbackData = std::string;
	};

	using AccessorType = accessorpp::Accessor<
		const int &,
		Policies
	>;

	struct ValuePair
	{
		int newValue;
		int oldValue;
		std::string context;
	};
	ValuePair changingValue {};
	ValuePair changedValue {};
	AccessorType accessor;
	accessor.onChanging() = [&changingValue, &accessor](const int newValue, const std::string & context) {
		changingValue.newValue = newValue;
		changingValue.oldValue = accessor;
		changingValue.context = context;
	};
	accessor.onChanged() = [&changedValue, &accessor]() {
		changedValue.newValue = accessor;
	};
	REQUIRE(changingValue.newValue == 0);
	REQUIRE(changingValue.oldValue == 0);
	REQUIRE(changedValue.newValue == 0);
	REQUIRE(changedValue.oldValue == 0);

	accessor = 3;
	REQUIRE(changingValue.newValue == 3);
	REQUIRE(changingValue.oldValue == 0);
	REQUIRE(changingValue.context == "");
	REQUIRE(changedValue.newValue == 3);
	REQUIRE(changedValue.oldValue == 0);

	accessor.setWithCallbackData(8, "Hello");
	REQUIRE(changingValue.newValue == 8);
	REQUIRE(changingValue.oldValue == 3);
	REQUIRE(changingValue.context == "Hello");
	REQUIRE(changedValue.newValue == 8);
	REQUIRE(changedValue.oldValue == 0);
}

