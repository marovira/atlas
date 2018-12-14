#include <atlas/math/Ray.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Checking Ray class", "[math]")
{
    using namespace atlas::math;

    Ray<Point> r;
    Ray<Point> r1{ Point{0.0f}, Point{0.0f} };
    REQUIRE((r == r1) == true);


}