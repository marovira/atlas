#include <atlas/math/Random.hpp>

#include <catch2/catch.hpp>

using namespace atlas::math;

TEST_CASE("[math] - Random<float>: get_random_in_range")
{
    Random<float> engine;

    auto r = engine.get_random_in_range(0.0f, 10.0f);
    REQUIRE(r >= 0.0f);
}

TEST_CASE("[math] - Random<int>: get_random_in_range")
{
    Random<int> engine;

    auto r = engine.get_random_in_range(0, 10);
    REQUIRE(r >= 0);
}
