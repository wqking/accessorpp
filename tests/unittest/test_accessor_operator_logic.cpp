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

TEST_CASE("Accessor, logic operator ==, !=, >, >=, <, <=")
{
	accessorpp::Accessor<int> accessor1;
	
	REQUIRE(accessor1 == 0);
	REQUIRE(accessor1 != 5);
	REQUIRE(accessor1 < 5);
	REQUIRE(accessor1 <= 0);
	REQUIRE(accessor1 > -1);
	REQUIRE(accessor1 >= 0);

	accessor1 = 5;
	REQUIRE(accessor1 == 5);
	REQUIRE(accessor1 != 0);

	accessorpp::Accessor<int> accessor2;
	REQUIRE(! (accessor1 == accessor2));
	REQUIRE(accessor1 != accessor2);
	REQUIRE(accessor1 > accessor2);
	REQUIRE(accessor1 >= accessor2);
	REQUIRE(accessor2 < accessor1);
	REQUIRE(accessor2 <= accessor1);
	accessor2 = 5;
	REQUIRE(accessor1 == accessor2);
}

TEST_CASE("Accessor, logic operator &&, ||")
{
	accessorpp::Accessor<int> accessor1;
	
	// The extra brackets () are to make CATCH happy, not required by Accessor.
	REQUIRE((accessor1 || 1));

	accessor1 = 5;
	REQUIRE((accessor1 && 1));

	accessorpp::Accessor<bool> accessor2;
	REQUIRE(accessor2 == false);
	accessor2 = true;
	const bool value = accessor1 || accessor2;
	REQUIRE(value);
	REQUIRE((accessor1 && accessor2));

}
