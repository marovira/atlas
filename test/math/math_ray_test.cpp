#include <atlas/math/glm.hpp>
#include <atlas/math/ray.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace atlas::math;

TEST_CASE("[Ray] - default constructor: math::glm::vec3", "[math]")
{
    Ray<glm::vec3> r;
    glm::vec3 pt{0.0f};

    REQUIRE(r.o == pt);
    REQUIRE(r.d == pt);
}

TEST_CASE("[Ray] - parametrized constructor: math::glm::vec3", "[math]")
{
    glm::vec3 o{0.0f}, d{1.0f};
    Ray<glm::vec3> r{o, d};

    REQUIRE(r.o == o);
    REQUIRE(r.d == d);
}

TEST_CASE("[Ray] - operator(): math::glm::vec3")
{
    glm::vec3 p{2.0f};
    Ray<glm::vec3> r{glm::vec3{1.0f}, glm::vec3{1.0f}};

    auto val = r(1.0f);
    REQUIRE(p == val);
}

TEST_CASE("[Ray] - operator==: math::glm::vec3", "[math]")
{
    Ray<glm::vec3> r, r1;

    REQUIRE(r == r1);

    r1.o = glm::vec3{1.0f};
    r1.d = glm::vec3{1.0f};

    REQUIRE_FALSE(r == r1);
}

TEST_CASE("[Ray] - operator!=: math::glm::vec3", "[math]")
{
    Ray<glm::vec3> r, r1;

    REQUIRE_FALSE(r != r1);

    r1.o = glm::vec3{1.0f};
    r1.d = glm::vec3{1.0f};

    REQUIRE(r != r1);
}
