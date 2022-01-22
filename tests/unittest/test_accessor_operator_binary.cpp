#include "test.h"
#include "accessorpp/accessor.h"

#include <iostream>

TEST_CASE("test accessor operator")
{
	int value = 0;
	accessorpp::Accessor<int> accessor1(&value);
	REQUIRE(accessor1 == 0);
	accessor1 = 5;
	REQUIRE(accessor1 == 5);

	int value2 = 0;
	accessorpp::Accessor<int> accessor2(&value2, &value2);
	REQUIRE(accessor2 == 0);
	accessor2 = 5;
	REQUIRE(accessor2 == 5);

	accessor2 += 3;
	REQUIRE(accessor2 == 8);
	accessor2 += accessor1;
	REQUIRE(accessor2 == 13);
	accessor2 -= 3;
	REQUIRE(accessor2 == 10);
	accessor2 -= accessor1;
	REQUIRE(accessor2 == 5);
	accessor2 *= 3;
	REQUIRE(accessor2 == 15);
	accessor2 *= accessor1;
	REQUIRE(accessor2 == 75);
	accessor2 /= 3;
	REQUIRE(accessor2 == 25);
	accessor2 /= accessor1;
	REQUIRE(accessor2 == 5);
	accessor2 &= 3;
	REQUIRE(accessor2 == 1);
	accessor2 |= 2;
	REQUIRE(accessor2 == 3);
	accessor2 &= accessor1;
	REQUIRE(accessor2 == 1);
	accessor2 |= accessor1;
	REQUIRE(accessor2 == 5);
	accessor2 ^= 3;
	REQUIRE(accessor2 == 6);
	accessor2 ^= accessor1;
	REQUIRE(accessor2 == 3);

	std::cout << (accessor2 ^ 0) << std::endl;

	std::string value3;
	accessorpp::Accessor<std::string> accessor3(&value3);
	REQUIRE(accessor3 == "");
	accessor3 += "a";
	REQUIRE(accessor3 == "a");
}

