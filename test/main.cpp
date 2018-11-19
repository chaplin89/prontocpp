#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Ensure that 1 is 1") {
	REQUIRE(1 == 1);
}