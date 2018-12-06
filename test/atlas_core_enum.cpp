#include <atlas/core/Enum.hpp>

#include <catch2/catch.hpp>

enum class Test : int
{
    A = 0,
    B, 
    C
};

TEST_CASE("Checking enum class conversion", "[core]")
{
    using atlas::core::enumToUnderlyingType;
    Test a{ Test::A };
    Test b{ Test::B };
    Test c{ Test::C };

    auto var = enumToUnderlyingType(a);
    REQUIRE(var == 0);

    var = enumToUnderlyingType(b);
    REQUIRE(var == 1);

    var = enumToUnderlyingType(c);
    REQUIRE(var == 2);

}
