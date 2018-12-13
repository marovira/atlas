#include <atlas/math/Coordinates.hpp>
#include <atlas/core/Float.hpp>

#include <catch2/catch.hpp>

using atlas::core::areEqual;


TEST_CASE("Checking Coordinate Functions", "[math]")
{
    using namespace atlas::math;

    Point p1{ 1, 0, 0 };
    Point p2{ 1, 1, 0 };

    auto s1 = cartesianToSpherical(p1);
    auto s2 = cartesianToSpherical(p2);

    REQUIRE(areEqual(s1.x, 1.0f));
    REQUIRE(areEqual(s1.y, 0.0f));
    REQUIRE(areEqual(s1.z, glm::radians(90.0f)));

    REQUIRE(areEqual(s2.x, glm::sqrt(2.0f)));
    REQUIRE(areEqual(s2.y, glm::radians(45.0f)));
    REQUIRE(areEqual(s2.z, glm::radians(90.0f)));

    auto c1 = sphericalToCartesian(s1);
    auto c2 = sphericalToCartesian(s2);

    REQUIRE(areEqual(c1.x, 1.0f));
    REQUIRE(areEqual(c1.y, 0.0f));
    REQUIRE(areEqual(c1.z, 0.0f));

    REQUIRE(areEqual(c2.x, 1.0f));
    REQUIRE(areEqual(c2.y, 1.0f));
    REQUIRE(areEqual(c2.z, 0.0f));
}