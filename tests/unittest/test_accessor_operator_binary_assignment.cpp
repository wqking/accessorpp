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

TEST_CASE("Accessor, int, binary assignment operator +=")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 15;
	accessor2 = 3;

	accessor1 += accessor2;
	REQUIRE(accessor1 == 18);
	REQUIRE(accessor2 == 3);

	accessor1 += 3;
	REQUIRE(accessor1 == 21);
}

TEST_CASE("Accessor, int, binary assignment operator -=")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 15;
	accessor2 = 3;

	accessor1 -= accessor2;
	REQUIRE(accessor1 == 12);
	REQUIRE(accessor2 == 3);

	accessor1 -= 3;
	REQUIRE(accessor1 == 9);
}

TEST_CASE("Accessor, int, binary assignment operator *=")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 15;
	accessor2 = 3;

	accessor1 *= accessor2;
	REQUIRE(accessor1 == 45);
	REQUIRE(accessor2 == 3);

	accessor1 *= 3;
	REQUIRE(accessor1 == 135);
}

TEST_CASE("Accessor, int, binary assignment operator /=")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 45;
	accessor2 = 3;

	accessor1 /= accessor2;
	REQUIRE(accessor1 == 15);
	REQUIRE(accessor2 == 3);

	accessor1 /= 3;
	REQUIRE(accessor1 == 5);
}

TEST_CASE("Accessor, int, binary assignment operator %=")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 16;
	accessor2 = 3;

	accessor1 %= accessor2;
	REQUIRE(accessor1 == 1);
	REQUIRE(accessor2 == 3);

	accessor1 = 8;
	accessor1 %= 3;
	REQUIRE(accessor1 == 2);
}

TEST_CASE("Accessor, int, binary assignment operator &=")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 10; // 0b1010
	accessor2 = 6; // 0b0110

	accessor1 &= accessor2;
	REQUIRE(accessor1 == 2);
	REQUIRE(accessor2 == 6);

	accessor1 = 7;
	accessor1 &= 3;
	REQUIRE(accessor1 == 3);
}

TEST_CASE("Accessor, int, binary assignment operator |=")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 10; // 0b1010
	accessor2 = 6; // 0b0110

	accessor1 |= accessor2;
	REQUIRE(accessor1 == 14);
	REQUIRE(accessor2 == 6);

	accessor1 |= 1;
	REQUIRE(accessor1 == 15);
}

TEST_CASE("Accessor, int, binary assignment operator ^=")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 10; // 0b1010
	accessor2 = 6; // 0b0110

	accessor1 ^= accessor2;
	REQUIRE(accessor1 == 12);
	REQUIRE(accessor2 == 6);

	accessor1 ^= 6;
	REQUIRE(accessor1 == 10);
}

TEST_CASE("Accessor, int, binary assignment operator <<=")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 14; // 0b1110
	accessor2 = 2;

	accessor1 <<= accessor2;
	REQUIRE(accessor1 == 56);
	REQUIRE(accessor2 == 2);

	accessor1 <<= 1;
	REQUIRE(accessor1 == 112);
}

TEST_CASE("Accessor, int, binary assignment operator >>=")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 14; // 0b1110
	accessor2 = 2;

	accessor1 >>= accessor2;
	REQUIRE(accessor1 == 3);
	REQUIRE(accessor2 == 2);

	accessor1 >>= 1;
	REQUIRE(accessor1 == 1);
}


