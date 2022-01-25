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
	accessorpp::Accessor<int> accessor;

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

