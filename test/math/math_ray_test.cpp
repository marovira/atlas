#include <atlas/math/math.hpp>
#include <atlas/math/ray.hpp>

#include <catch2/catch.hpp>

using namespace atlas::math;

TEST_CASE("[Ray] - default constructor: math::Point", "[math]")
{
    Ray<Point> r;
    Point pt{0.0f};

    REQUIRE(r.o == pt);
    REQUIRE(r.d == pt);
}

TEST_CASE("[Ray] - parametrized constructor: math::Point", "[math]")
{
    Point o{0.0f}, d{1.0f};
    Ray<Point> r{o, d};

    REQUIRE(r.o == o);
    REQUIRE(r.d == d);
}

TEST_CASE("[Ray] - operator(): math::Point")
{
    Point p{2.0f};
    Ray<Point> r{Point{1.0f}, Point{1.0f}};

    auto val = r(1.0f);
    REQUIRE(p == val);
}

TEST_CASE("[Ray] - operator==: math::Point", "[math]")
{
    Ray<Point> r, r1;

    REQUIRE(r == r1);

    r1.o = Point{1.0f};
    r1.d = Point{1.0f};

    REQUIRE_FALSE(r == r1);
}

TEST_CASE("[Ray] - operator!=: math::Point", "[math]")
{
    Ray<Point> r, r1;

    REQUIRE_FALSE(r != r1);

    r1.o = Point{1.0f};
    r1.d = Point{1.0f};

    REQUIRE(r != r1);
}
