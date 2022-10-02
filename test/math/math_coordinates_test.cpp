#include <atlas/math/coordinates.hpp>
#include <zeus/float.hpp>

#include <catch2/catch_test_macros.hpp>

using zeus::are_equal;
using namespace atlas::math;

TEST_CASE("[coordinates] - cartesian_to_spherical", "[math]")
{
    glm::vec3 p1{1, 0, 0};
    glm::vec3 p2{1, 1, 0};

    auto s1 = cartesian_to_spherical(p1);
    auto s2 = cartesian_to_spherical(p2);

    REQUIRE(are_equal(s1.x, 1.0f));
    REQUIRE(are_equal(s1.y, 0.0f));
    REQUIRE(are_equal(s1.z, glm::radians(90.0f)));

    REQUIRE(are_equal(s2.x, glm::sqrt(2.0f)));
    REQUIRE(are_equal(s2.y, glm::radians(45.0f)));
    REQUIRE(are_equal(s2.z, glm::radians(90.0f)));
}

TEST_CASE("[coordinates] - spherical_to_cartesian", "[math]")
{
    glm::vec3 p1{1, 0, 0};
    glm::vec3 p2{1, 1, 0};

    auto s1 = cartesian_to_spherical(p1);
    auto s2 = cartesian_to_spherical(p2);

    auto c1 = spherical_to_cartesian(s1);
    auto c2 = spherical_to_cartesian(s2);

    REQUIRE(are_equal(c1.x, 1.0f));
    REQUIRE(are_equal(c1.y, 0.0f));
    REQUIRE(are_equal(c1.z, glm::cos(glm::radians(90.0f))));

    REQUIRE(are_equal(c2.x, 1.0f));
    REQUIRE(are_equal(c2.y, 1.0f));
    REQUIRE(are_equal(c2.z, glm::sqrt(2.0f) * glm::cos(glm::radians(90.0f))));
}

TEST_CASE("[coordinates] - cartesian_to_cylindrical", "[math]")
{
    glm::vec3 p1{1, 0, 0};
    glm::vec3 p2{1, 1, 0};

    auto x1 = cartesian_to_cylindrical(p1);
    auto x2 = cartesian_to_cylindrical(p2);

    REQUIRE(are_equal(x1.x, 1.0f));
    REQUIRE(are_equal(x1.y, 0.0f));
    REQUIRE(are_equal(x1.z, 0.0f));

    REQUIRE(are_equal(x2.x, glm::sqrt(2.0f)));
    REQUIRE(are_equal(x2.y, glm::radians(45.0f)));
    REQUIRE(are_equal(x2.z, 0.0f));
}

TEST_CASE("[coordinates] - cylindrical_to_cartesian", "[math]")
{
    glm::vec3 p1{1, 0, 0};
    glm::vec3 p2{1, 1, 0};

    auto x1 = cartesian_to_cylindrical(p1);
    auto x2 = cartesian_to_cylindrical(p2);

    auto c1 = cylindrical_to_cartesian(x1);
    auto c2 = cylindrical_to_cartesian(x2);

    REQUIRE(are_equal(c1.x, 1.0f));
    REQUIRE(are_equal(c1.y, 0.0f));
    REQUIRE(are_equal(c1.z, 0.0f));

    REQUIRE(are_equal(c2.x, 1.0f));
    REQUIRE(are_equal(c2.y, 1.0f));
    REQUIRE(are_equal(c2.z, 0.0f));
}

TEST_CASE("[coordinates] - cartesian_to_polar", "[math]")
{
    glm::vec2 p1{1, 0};
    glm::vec2 p2{1, 1};

    auto x1 = cartesian_to_polar(p1);
    auto x2 = cartesian_to_polar(p2);

    REQUIRE(are_equal(x1.x, 1.0f));
    REQUIRE(are_equal(x1.y, 0.0f));

    REQUIRE(are_equal(x2.x, glm::sqrt(2.0f)));
    REQUIRE(are_equal(x2.y, glm::radians(45.0f)));
}

TEST_CASE("[coordinates] - polar_to_cartesian", "[math]")
{
    glm::vec2 p1{1, 0};
    glm::vec2 p2{1, 1};

    auto x1 = cartesian_to_polar(p1);
    auto x2 = cartesian_to_polar(p2);

    auto c1 = polar_to_cartesian(x1);
    auto c2 = polar_to_cartesian(x2);

    REQUIRE(are_equal(c1.x, 1.0f));
    REQUIRE(are_equal(c1.y, 0.0f));

    REQUIRE(are_equal(c2.x, 1.0f));
    REQUIRE(are_equal(c2.y, 1.0f));
}
