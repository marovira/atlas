#include <atlas/core/Float.hpp>

#include <catch2/catch.hpp>

#include <cmath>

template <typename T>
inline constexpr T epsilon()
{
    return static_cast<T>(0.00001);
}

TEST_CASE("Checking areEqual", "[core]")
{
    using atlas::core::areEqual;

    double d1 = 0.2;
    double d2 = 1 / std::sqrt(5) / std::sqrt(5);
    REQUIRE(areEqual(d1, d2) == true);

    float f1 = 0.2f;
    float f2 = 1.0f / std::sqrt(5.0f) / std::sqrt(5.0f);
    REQUIRE(areEqual(f1, f2) == true);

    REQUIRE(areEqual<double, epsilon>(d1, d2) == true);
    REQUIRE(areEqual<float, epsilon>(f1, f2) == true);
}

TEST_CASE("Checking isZero", "[core]")
{
    using atlas::core::isZero;

    double d1 = 1.0 - (std::sqrt(5.0) / std::sqrt(5.0));
    REQUIRE(isZero(d1) == true);

    float f1 = 1.0f - (std::sqrt(5.0f) / std::sqrt(5.0f));
    REQUIRE(isZero(f1) == true);

    REQUIRE(isZero<double, epsilon>(d1));
    REQUIRE(isZero<float, epsilon>(f1));
}

TEST_CASE("Checking geq", "[core]")
{
    using atlas::core::geq;

    double d1 = 0.2;
    double d2 = 1 / std::sqrt(5) / std::sqrt(5);
    double d3 = 1.0 + d2;

    REQUIRE(geq(d1, d2) == true);
    REQUIRE(geq(d3, d2) == true);

    float f1 = 0.2f;
    float f2 = 1 / std::sqrt(5.0f) / std::sqrt(5.0f);
    float f3 = 1.0f + f2;

    REQUIRE(geq(f1, f2) == true);
    REQUIRE(geq(f3, f2) == true);

    REQUIRE(geq<double, epsilon>(d1, d2) == true);
    REQUIRE(geq<double, epsilon>(d3, d2) == true);
    REQUIRE(geq<float, epsilon>(f1, f2) == true);
    REQUIRE(geq<float, epsilon>(f3, f2) == true);
}

TEST_CASE("Checking leq", "[core]")
{
    using atlas::core::leq;

    double d1 = 0.2;
    double d2 = 1 / std::sqrt(5) / std::sqrt(5);
    double d3 = 1.0 + d2;

    REQUIRE(leq(d1, d2) == true);
    REQUIRE(leq(d2, d3) == true);

    float f1 = 0.2f;
    float f2 = 1 / std::sqrt(5.0f) / std::sqrt(5.0f);
    float f3 = 1.0f + f2;

    REQUIRE(leq(f1, f2) == true);
    REQUIRE(leq(f2, f3) == true);

    REQUIRE(leq<double, epsilon>(d1, d2) == true);
    REQUIRE(leq<double, epsilon>(d2, d3) == true);
    REQUIRE(leq<float, epsilon>(f1, f2) == true);
    REQUIRE(leq<float, epsilon>(f2, f3) == true);
}
