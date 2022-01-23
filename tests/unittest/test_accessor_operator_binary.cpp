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

TEST_CASE("Accessor, int, binary operator +, -, *, /, %, &, |, ^, <<, >>")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 15;
	accessor2 = 3;
	REQUIRE(accessor1 == 15);
	REQUIRE(accessor2 == 3);

	REQUIRE(accessor1 + accessor2 == 18);
	REQUIRE(accessor1 + 3 == 18);
	REQUIRE(accessor1 == 15);
	REQUIRE(accessor2 == 3);
	REQUIRE((accessor1 + accessor2).get() == 18);
	REQUIRE((accessor1 + 3).get() == 18);

	REQUIRE(accessor1 - accessor2 == 12);
	REQUIRE(accessor1 - 3 == 12);
	REQUIRE((accessor1 - accessor2).get() == 12);
	REQUIRE((accessor1 - 3).get() == 12);

	REQUIRE(accessor1 * accessor2 == 45);
	REQUIRE(accessor1 * 3== 45);
	REQUIRE((accessor1 * accessor2).get() == 45);
	REQUIRE((accessor1 * 3).get() == 45);

	REQUIRE(accessor1 / accessor2 == 5);
	REQUIRE(accessor1 / 3 == 5);
	REQUIRE((accessor1 / accessor2).get() == 5);
	REQUIRE((accessor1 / 3).get() == 5);

	accessor1 = 10; // 0b1010
	accessor2 = 6; // 0b0110
	REQUIRE(accessor1 % accessor2 == 4);
	REQUIRE(accessor1 % 6 == 4);
	REQUIRE((accessor1 % accessor2).get() == 4);
	REQUIRE((accessor1 % 6).get() == 4);

	REQUIRE((accessor1 & accessor2) == 2);
	REQUIRE((accessor1 & 6) == 2);
	REQUIRE((accessor1 & accessor2).get() == 2);
	REQUIRE((accessor1 & 6).get() == 2);

	REQUIRE((accessor1 | accessor2) == 14);
	REQUIRE((accessor1 | 6) == 14);
	REQUIRE((accessor1 | accessor2).get() == 14);
	REQUIRE((accessor1 | 6).get() == 14);

	REQUIRE((accessor1 ^ accessor2) == 12);
	REQUIRE((accessor1 ^ 6) == 12);
	REQUIRE((accessor1 ^ accessor2).get() == 12);
	REQUIRE((accessor1 ^ 6).get() == 12);

	accessor1 = 14; // 0b1110
	accessor2 = 2;
	REQUIRE(accessor1 << accessor2 == 56);
	REQUIRE(accessor1 << 2 == 56);
	REQUIRE((accessor1 << accessor2).get() == 56);
	REQUIRE((accessor1 << 2).get() == 56);

	REQUIRE(accessor1 >> accessor2 == 3);
	REQUIRE(accessor1 >> 2 == 3);
	REQUIRE((accessor1 >> accessor2).get() == 3);
	REQUIRE((accessor1 >> 2).get() == 3);
}

