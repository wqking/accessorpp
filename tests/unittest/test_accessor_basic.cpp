#include "test.h"
#include "accessorpp/accessor.h"

#include <iostream>
#include <string>

TEST_CASE("Accessor, int, default storage, StoreValue")
{
	accessorpp::Accessor<int> accessor;
	REQUIRE(accessor.get() == 0);

	accessor = 3;
	REQUIRE(accessor.get() == 3);

	accessor = 8;
	REQUIRE(accessor.get() == 8);
}

TEST_CASE("Accessor, const int &, default storage, StoreValue")
{
	accessorpp::Accessor<const int &> accessor;
	REQUIRE(accessor.get() == 0);

	accessor = 3;
	REQUIRE(accessor.get() == 3);

	accessor = 8;
	REQUIRE(accessor.get() == 8);
}

TEST_CASE("Accessor, int &, default storage, StoreValue")
{
	accessorpp::Accessor<int &> accessor;
	REQUIRE(accessor.get() == 0);

	int value = 3;
	accessor = value;
	REQUIRE(accessor.get() == 3);

	value = 8;
	accessor = value;
	REQUIRE(accessor.get() == 8);
}

TEST_CASE("Accessor, 2")
{
	using AccessorType = accessorpp::Accessor<
		const int &,
		accessorpp::StoreValue,
		std::function<void (int, int)>
	>;
	AccessorType accessor;
	accessor.getCallback() = [](const int newValue, const int oldValue) {
		std::cout << "newValue=" << newValue << " oldValue=" << oldValue << std::endl;
	};
	accessor = 3;
	accessor = 8;
}
