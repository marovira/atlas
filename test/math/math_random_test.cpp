#include <atlas/math/random.hpp>

#include <catch2/catch.hpp>

using namespace atlas::math;

TEST_CASE("[Random] - get_random_in_range: float", "[math]")
{
    Random<float> engine;

    auto r = engine.get_random_in_range(0.0f, 10.0f);
    REQUIRE(r >= 0.0f);
}

TEST_CASE("[Random] - get_random_in_range: int", "[math]")
{
    Random<int> engine;

    auto r = engine.get_random_in_range(0, 10);
    REQUIRE(r >= 0);
}
