#include <atlas/math/Coordinates.hpp>
#include <zeus/float.hpp>

#include <catch2/catch.hpp>

using zeus::are_equal;

TEST_CASE("Checking Spherical Functions", "[math]")
{
    using namespace atlas::math;

    Point p1{1, 0, 0};
    Point p2{1, 1, 0};

    auto s1 = cartesianToSpherical(p1);
    auto s2 = cartesianToSpherical(p2);

    REQUIRE(are_equal(s1.x, 1.0f));
    REQUIRE(are_equal(s1.y, 0.0f));
    REQUIRE(are_equal(s1.z, glm::radians(90.0f)));

    REQUIRE(are_equal(s2.x, glm::sqrt(2.0f)));
    REQUIRE(are_equal(s2.y, glm::radians(45.0f)));
    REQUIRE(are_equal(s2.z, glm::radians(90.0f)));

    auto c1 = sphericalToCartesian(s1);
    auto c2 = sphericalToCartesian(s2);

    REQUIRE(are_equal(c1.x, 1.0f));
    REQUIRE(are_equal(c1.y, 0.0f));
    REQUIRE(are_equal(c1.z, glm::cos(glm::radians(90.0f))));

    REQUIRE(are_equal(c2.x, 1.0f));
    REQUIRE(are_equal(c2.y, 1.0f));
    REQUIRE(are_equal(c2.z, glm::sqrt(2.0f) * glm::cos(glm::radians(90.0f))));
}

TEST_CASE("Checking Cylindrical Functions", "[math]")
{
    using namespace atlas::math;

    Point p1{1, 0, 0};
    Point p2{1, 1, 0};

    auto x1 = cartesianToCylindrical(p1);
    auto x2 = cartesianToCylindrical(p2);

    REQUIRE(are_equal(x1.x, 1.0f));
    REQUIRE(are_equal(x1.y, 0.0f));
    REQUIRE(are_equal(x1.z, 0.0f));

    REQUIRE(are_equal(x2.x, glm::sqrt(2.0f)));
    REQUIRE(are_equal(x2.y, glm::radians(45.0f)));
    REQUIRE(are_equal(x2.z, 0.0f));

    auto c1 = cylindricalToCartesian(x1);
    auto c2 = cylindricalToCartesian(x2);

    REQUIRE(are_equal(c1.x, 1.0f));
    REQUIRE(are_equal(c1.y, 0.0f));
    REQUIRE(are_equal(c1.z, 0.0f));

    REQUIRE(are_equal(c2.x, 1.0f));
    REQUIRE(are_equal(c2.y, 1.0f));
    REQUIRE(are_equal(c2.z, 0.0f));
}

TEST_CASE("Checking Polar Functions", "[math]")
{
    using namespace atlas::math;

    Point2 p1{1, 0};
    Point2 p2{1, 1};

    auto x1 = cartesianToPolar(p1);
    auto x2 = cartesianToPolar(p2);

    REQUIRE(are_equal(x1.x, 1.0f));
    REQUIRE(are_equal(x1.y, 0.0f));

    REQUIRE(are_equal(x2.x, glm::sqrt(2.0f)));
    REQUIRE(are_equal(x2.y, glm::radians(45.0f)));

    auto c1 = polarToCartesian(x1);
    auto c2 = polarToCartesian(x2);

    REQUIRE(are_equal(c1.x, 1.0f));
    REQUIRE(are_equal(c1.y, 0.0f));

    REQUIRE(are_equal(c2.x, 1.0f));
    REQUIRE(are_equal(c2.y, 1.0f));
}
