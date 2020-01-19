#include <atlas/core/Float.hpp>
#include <atlas/math/Solvers.hpp>

#include <catch2/catch.hpp>

using namespace atlas::math;
using atlas::core::areEqual;

TEST_CASE("Testing quadratic solver", "[math]")
{
    // x^2 - 1 = 0.
    //  Two roots: 1, -1.
    {
        std::vector<double> coefficients{1.0, 0.0, -1.0};
        std::vector<double> roots;
        auto numRoots = solveQuadratic(coefficients, roots);

        REQUIRE(numRoots == 2);
        REQUIRE(areEqual(roots[0], 1.0));
        REQUIRE(areEqual(roots[1], -1.0));
    }

    // x^2 - 2x + 1.
    // One root: 1.
    {
        std::vector<double> coefficients{1.0, -2.0, 1.0};
        std::vector<double> roots;
        auto numRoots = solveQuadratic(coefficients, roots);

        REQUIRE(numRoots == 1);
        REQUIRE(areEqual(roots[0], 1.0));
    }

    // x^2 + 1.
    // No real roots.
    {
        std::vector<double> coefficients{1.0, 0.0, 1.0};
        std::vector<double> roots;
        auto numRoots = solveQuadratic(coefficients, roots);

        REQUIRE(numRoots == 0);
    }
}

TEST_CASE("Testing cubic solver", "[math]")
{
    // -6 + 11x -6x^2 + x^3.
    // 3 roots: 1, 2, 3.
    {
        std::vector<double> coefficients{-6.0, 11.0, -6.0, 1.0};
        std::vector<double> roots;
        auto numRoots = solveCubic(coefficients, roots);

        REQUIRE(numRoots == 3);
        REQUIRE(areEqual(roots[0], 3.0));
        REQUIRE(areEqual(roots[1], 2.0));
        REQUIRE(areEqual(roots[2], 1.0));
    }

    // -x^2 + x^3.
    // 2 roots.
    {
        std::vector<double> coefficients{0.0, 0.0, -1.0, 1.0};
        std::vector<double> roots;
        auto numRoots = solveCubic(coefficients, roots);

        REQUIRE(numRoots == 2);
        REQUIRE(areEqual(roots[0], 1.0));
        REQUIRE(areEqual(roots[1], 0.0));
    }

    // -1 + x^3
    // 1 real root, two complex.
    {
        std::vector<double> coefficients{-1.0, 0.0, 0.0, 1.0};
        std::vector<double> roots;
        auto numRoots = solveCubic(coefficients, roots);

        REQUIRE(numRoots == 1);
        REQUIRE(areEqual(roots[0], 1.0));
    }

    // -1 + 3x - 3x^2 + x^3.
    // 1 root.
    {
        std::vector<double> coefficients{-1.0, 3.0, -3.0, 1.0};
        std::vector<double> roots;
        auto numRoots = solveCubic(coefficients, roots);

        REQUIRE(numRoots == 1);
        REQUIRE(areEqual(roots[0], 1.0));
    }
}

TEST_CASE("Testing quartic solver", "[math]")
{
    // 24 + -50x + 35x^2 - 10x^3 + x^4
    // 4 roots: 1, 2, 3, 4.
    {
        std::vector<double> coefficients{24.0, -50.0, 35.0, -10.0, 1.0};
        std::vector<double> roots;
        auto numRoots = solveQuartic(coefficients, roots);

        REQUIRE(numRoots == 4);
        REQUIRE(areEqual(roots[0], 2.0));
        REQUIRE(areEqual(roots[1], 1.0));
        REQUIRE(areEqual(roots[2], 4.0));
        REQUIRE(areEqual(roots[3], 3.0));
    }

    // 2x^2 - 3x^3 + x^4.
    // 3 roots: 0, 1, 2.
    {
        std::vector<double> coefficients{0.0, 0.0, 2.0, -3.0, 1.0};
        std::vector<double> roots;
        auto numRoots = solveQuartic(coefficients, roots);

        REQUIRE(numRoots == 3);
        REQUIRE(areEqual(roots[0], 2.0));
        REQUIRE(areEqual(roots[1], 1.0));
        REQUIRE(areEqual(roots[2], 0.0));
    }

    // 2 real roots, 2 complex.
    // 2 -3x + 3x^2 - 3x^3 + x^4
    {
        std::vector<double> coefficients{2.0, -3.0, 3.0, -3.0, 1.0};
        std::vector<double> roots;
        auto numRoots = solveQuartic(coefficients, roots);

        REQUIRE(numRoots == 2);
        REQUIRE(areEqual(roots[0], 2.0));
        REQUIRE(areEqual(roots[1], 1.0));
    }

    // 1 - 2x^2 + x^4.
    // 2 real roots.
    {
        std::vector<double> coefficients{1.0, 0.0, -2.0, 0.0, 1.0};
        std::vector<double> roots;
        auto numRoots = solveQuartic(coefficients, roots);

        REQUIRE(numRoots == 2);
        REQUIRE(areEqual(roots[0], -1.0));
        REQUIRE(areEqual(roots[1], 1.0));
    }

    // 1 - 4x + 6x^2 - 4x^3 + x^4.
    // 1 root.
    {
        std::vector<double> coefficients{1.0, -4.0, 6.0, -4.0, 1.0};
        std::vector<double> roots;
        auto numRoots = solveQuartic(coefficients, roots);

        // Due to epsilon values, roots may be reported more than once.
        REQUIRE((numRoots == 1 || numRoots == 2));

        if (numRoots == 1)
        {
            REQUIRE(areEqual(roots[0], 1.0));
        }
        else
        {
            REQUIRE(areEqual(roots[0], 1.0));
            REQUIRE(areEqual(roots[1], 1.0));
        }
    }
}
