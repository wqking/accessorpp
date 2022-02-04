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

// Include the head
#include "accessorpp/accessor.h"

#include "tutorial.h"

#include <iostream>

TEST_CASE("Accessor tutorial 1, basic")
{
	std::cout << std::endl << "Accessor tutorial 1, basic" << std::endl;

	// The namespace is accessor.
	// The first parameter is the type of the underlying value.
	// By default, the accessor stores the value in internal storage.
	// The argument 0 is the initial value, it can be omitted
	// and access will use the default value, i.e, 0 for int, "" for string, etc.
	accessorpp::Accessor<int> accessor(0);
	// The argument can be omitted, and the code will look lik,
	// accessorpp::Accessor<int> accessor;

	// To obtain the underlying value, we can cast the accessor to the type
	std::cout << "Accessor tutorial 1: the default value should be 0, got " << (int)accessor << std::endl;

	// Now let's set a new value to the accessor using the assignment operator = .
	accessor = 3;
	// We can also call Accessor::get() to obtain the underlying value
	std::cout << "Accessor tutorial 1: the new value should be 3, got " << accessor.get() << std::endl;

	// We can also call Accessor::set() to set the underlying value.
	accessor = 8;
	// Accessor supports stream operator directly, so we don't need to obtain the underlying value.
	std::cout << "Accessor tutorial 1: the new value should be 8, got " << accessor << std::endl;
}

TEST_CASE("Accessor tutorial 2, customized getter/setter")
{
	std::cout << std::endl << "Accessor tutorial 2, customized getter/setter" << std::endl;

	{
		// Let's create an accessor with customized getter and setter.
		// The first argument is the getter, the second is the setter.
		// Note: the code requires C++20 standard in MSVC, otherwise it gives error "lambda capture variable not found".
		// The code works with GCC and Clang.
		accessorpp::Accessor<int> accessor(
			// This is the getter
			[&accessor]() { return accessor.directGet();  },
			// This is the setter, it multiplies the incoming value with 2 and store to the accessor, for fun.
			[&accessor](const int value) { accessor.directSet(value * 2);  }
		);
		accessor = 3;
		std::cout << "Accessor tutorial 2: the accessor is set with 3, now the value should be 6, got " << (int)accessor << std::endl;
	}

	{
		// Now let's create an accessor with default getter and customized setter.
		// The first argument is accessorpp::defaultGetter, it uses the getter supplied by the accessor,
		// the second is the setter, the third argument is the inital value.
		// Likesie, there is accessorpp::defaultSetter for the default getter.
		// This time we use the syntax that MSVC is happy with.
		accessorpp::Accessor<int> * ptr;
		accessorpp::Accessor<int> accessor(
			accessorpp::defaultGetter,
			// Must capture ptr as reference, otherwise it won't get the value set later.
			[&ptr](const int value) { ptr->directSet(value + 1);  },
			5
		);
		ptr = &accessor;
		std::cout << "Accessor tutorial 2: the initial accessor is 5, got " << (int)accessor << std::endl;
		accessor = 8;
		std::cout << "Accessor tutorial 2: the accessor is set with 8, now the value should be 9, got " << (int)accessor << std::endl;
	}

	{
		// Now let's create a read-only accessor with default getter.
		// The first argument is accessorpp::defaultGetter, it uses the getter supplied by the accessor,
		// the second is accessorpp::noSetter, it means no setter available, thus the access is read-only.
		accessorpp::Accessor<int> accessor(
			accessorpp::defaultGetter,
			accessorpp::noSetter,
			5
		);
		std::cout << std::boolalpha;
		std::cout << "Accessor tutorial 2: the initial accessor is 5, got " << (int)accessor << std::endl;
		std::cout << "Accessor tutorial 2: isReadOnly() should be true, got " << accessor.isReadOnly() << std::endl;
		try {
			// Assigning to read-only accessor will throw exception std::logic_error
			accessor = 8;
		}
		catch(const std::logic_error &) {
			std::cout << "Accessor tutorial 2: set to the read-only accessor causes std::logic_error, we got here." << std::endl;
		}
	}
}

TEST_CASE("Accessor tutorial 3, external storage")
{
	std::cout << std::endl << "Accessor tutorial 3, external storage" << std::endl;

	// To use external storage, we need to define the policies which is a struct.
	struct MyPolicies
	{
		// The type Storage determines how the value is stored.
		// If Storage is accessorpp::InternalStorage, the Accessor stores the value inside the Accessor.
		// If Storage is accessorpp::ExternalStorage, the Accessor doesn't store the value, and
		// how the underlying value is accessed completely depends on the getter and setter.
		using Storage = accessorpp::ExternalStorage;
	};

	int value = 38;
	// Pass MyPolicies as the second template argument, and pass &value as the getter and setter.
	// The access will read from 'value', and set to 'value'.
	accessorpp::Accessor<int, MyPolicies> accessor(&value, &value);
	std::cout << "Accessor tutorial 3: the initial value is 38, accessor = " << (int)accessor << std::endl;
	accessor = 8;
	std::cout
		<< "Accessor tutorial 3: the accessor is set with 8, now both value and accessor should be 8, got"
		<< " value = " << value << " and accessor = " << (int)accessor
		<< std::endl;
}

TEST_CASE("Accessor tutorial 4, on change event")
{
	std::cout << std::endl << "Accessor tutorial 4, on change event" << std::endl;

	// To use onChanging and onChanged events, we need to define the policies which is a struct.
	struct MyPolicies
	{
		// Use std::function<void (int)> as OnChangingCallback,
		// and leave OnChangedCallback default which will not trigger onChanged event.
		// Here we use std::function for demostration purpose, you should use the CallbackList class in eventpp library,
		// https://github.com/wqking/eventpp
		using OnChangingCallback = std::function<void (int)>;
	};
	accessorpp::Accessor<int, MyPolicies> accessor;
	// Assign the onChanging event callback. If we use eventpp::CallbackList here,
	// we can add multiple callbacks to the event.
	accessor.onChanging() = [&accessor](const int newValue) {
		std::cout << "Accessor tutorial 4: the onChanging event got new value " << newValue
			<< " and old value is " << accessor << std::endl;
	};

	// In onChanging, the newValue will be 8, and old value is 0
	accessor = 8;
	// In onChanging, the newValue will be 38, and old value is 8
	accessor = 38;
}

