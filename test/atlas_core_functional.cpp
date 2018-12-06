#include <atlas/core/Functional.hpp>

#include <catch2/catch.hpp>

#include <vector>

template <typename T>
inline bool vectorEqual(std::vector<T> const& v1, std::vector<T> const& v2)
{
    if (v1.size() != v2.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < v1.size(); ++i)
    {
        if (v1[i] != v2[i])
        {
            return false;
        }
    }

    return true;
}

TEST_CASE("Checking iota", "[core]")
{
    using atlas::core::iota;

    std::vector<int> v1(10);
    std::vector<float> v2(10);
    std::vector<int> c1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::vector<float> c2{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f };

    iota(v1.begin(), v1.end(), 1, 1);
    iota(v2.begin(), v2.end(), 1.0f, 1.0f);

    REQUIRE(vectorEqual(v1, c1));
    REQUIRE(vectorEqual(v2, c2));
}