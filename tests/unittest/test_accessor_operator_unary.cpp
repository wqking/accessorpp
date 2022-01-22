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

