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

TEST_CASE("Accessor, int, unary operator ++")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 8;
	accessor2 = ++accessor1;
	REQUIRE(accessor1 == 9);
	REQUIRE(accessor2 == 9);
}

TEST_CASE("Accessor, int, unary operator --")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 8;
	accessor2 = accessor1++;
	REQUIRE(accessor1 == 9);
	REQUIRE(accessor2 == 8);
}

TEST_CASE("Accessor, int, unary operator !")
{
	accessorpp::Accessor<bool> accessor1;
	accessorpp::Accessor<bool> accessor2;

	accessor1 = true;
	accessor2 = ! accessor1;
	REQUIRE(accessor1);
	REQUIRE(! accessor2);

	accessor2 = ! accessor2;
	REQUIRE(accessor1);
	REQUIRE(accessor2);
}

TEST_CASE("Accessor, int, unary operator +")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = -5;
	accessor2 = +accessor1;
	REQUIRE(accessor1 == -5);
	REQUIRE(accessor2 == -5);

	accessor2 = -8;
	accessor2 = +accessor2;
	REQUIRE(accessor1 == -5);
	REQUIRE(accessor2 == -8);
}

TEST_CASE("Accessor, int, unary operator -")
{
	accessorpp::Accessor<int> accessor1;
	accessorpp::Accessor<int> accessor2;

	accessor1 = 5;
	accessor2 = -accessor1;
	REQUIRE(accessor1 == 5);
	REQUIRE(accessor2 == -5);

	accessor2 = 8;
	accessor2 = -accessor2;
	REQUIRE(accessor1 == 5);
	REQUIRE(accessor2 == -8);
}

