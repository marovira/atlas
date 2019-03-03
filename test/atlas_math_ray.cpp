#include <atlas/math/Ray.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Checking Ray class", "[math]")
{
    using namespace atlas::math;

    Ray<Point> r;
    Ray<Point> r1{Point{0.0f}, Point{0.0f}};
    REQUIRE((r == r1) == true);
    REQUIRE((r != r1) == false);

    r1.o = Point{1.0f};
    r1.d = Point{1.0f};

    REQUIRE((r == r1) == false);
    REQUIRE((r != r1) == true);

    Point p{2.0f};
    auto  val = r1(1.0f);
    REQUIRE((val == p) == true);
}