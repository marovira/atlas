#include <atlas/math/Random.hpp>

#include <catch2/catch.hpp>

using namespace atlas::math;

TEST_CASE("Checking Random<float> class", "[math]")
{
    Random<float> engine;

    auto r = engine.getRandomRange(0.0f, 10.0f);
    REQUIRE(r >= 0.0f);
}

TEST_CASE("Checking Random<int> class", "[math]")
{
    Random<int> engine;

    auto r = engine.getRandomRange(0, 10);
    REQUIRE(r >= 0);
}
