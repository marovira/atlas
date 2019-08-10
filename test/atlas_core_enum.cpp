#include <atlas/core/Enum.hpp>

#include <catch2/catch.hpp>

enum class TypeTest : int
{
    A = 0,
    B,
    C
};

enum class OpTest : unsigned char
{
    A = 0x00,
    B = 0x01,
    C = 0x10,
    D = 0x11,
    E = 0xFF
};

TEST_CASE("Checking enum class conversion", "[core]")
{
    using atlas::core::enumToUnderlyingType;
    TypeTest a{TypeTest::A};
    TypeTest b{TypeTest::B};
    TypeTest c{TypeTest::C};

    auto var = enumToUnderlyingType(a);
    REQUIRE(var == 0);

    var = enumToUnderlyingType(b);
    REQUIRE(var == 1);

    var = enumToUnderlyingType(c);
    REQUIRE(var == 2);
}

TEST_CASE("Checking enum operator ~", "[core]")
{
    using namespace atlas::core;

    OpTest a{OpTest::A};
    OpTest var;
    var = ~a;
    REQUIRE(var == OpTest::E);
}

TEST_CASE("Checking enum operator |", "[core]")
{
    using namespace atlas::core;

    OpTest a{OpTest::B};
    OpTest b{OpTest::C};
    OpTest var;
    var = a | b;
    REQUIRE(var == OpTest::D);
}

TEST_CASE("Checking enum operator &", "[core]")
{
    using namespace atlas::core;

    OpTest a{OpTest::B};
    OpTest b{OpTest::C};
    OpTest var;
    var = a & b;
    REQUIRE(var == OpTest::A);
}

TEST_CASE("Checking enum operator ^", "[core]")
{
    using namespace atlas::core;

    OpTest a{OpTest::B};
    OpTest b{OpTest::C};
    OpTest var;
    var = a ^ b;
    REQUIRE(var == OpTest::D);
}
