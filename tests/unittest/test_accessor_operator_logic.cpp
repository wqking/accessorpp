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
