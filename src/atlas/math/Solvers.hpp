#pragma once

#include <atlas/core/Float.hpp>

#include <cassert>
#include <cmath>
#include <vector>

namespace atlas::math
{
    /**
     * Solves a quadratic equation of the form Ax^2 + Bx + C.
     * Note that the coefficients need to be passed in reverse, so {C, B, A}.
     *
     * @param coeffs The coefficients of the quadratic equation in reverse.
     * @param roots The roots of the equation.
     * @return The number of real roots (if any).
     */
    template<typename T,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    std::size_t solveQuadratic(std::vector<T> const& coeffs,
                               std::vector<T>& roots)
    {
        using atlas::core::isZero;

        assert(coeffs.size() >= 3);

        if (roots.empty())
        {
            roots.resize(2);
        }

        // Quadratic: x^2 + px + q = 0
        T p, q, D;
        p = coeffs[1] / (T{2} * coeffs[2]);
        q = coeffs[0] / coeffs[2];

        D = p * p - q;

        if (isZero<T>(D))
        {
            roots[0] = -p;
            return 1;
        }
        else if (D > T{0})
        {
            T sqrtD  = std::sqrt(D);
            roots[0] = sqrtD - p;
            roots[1] = -sqrtD - p;
            return 2;
        }

        return 0;
    }

    /**
     * Solves a cubic equation of the form Ax^3 + Bx^2 + Cx + D. Note that the
     * coefficients need to be passed in reverse, so {D, C, B, A}.
     *
     * @param coeffs The coefficients of the quadratic equation in reverse.
     * @param roots The roots of the equation.
     * @return The number of real roots (if any).
     */
    template<typename T,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    std::size_t solveCubic(std::vector<T> const& coeffs, std::vector<T>& roots)
    {
        using atlas::core::isZero;

        assert(coeffs.size() >= 4);

        if (roots.empty())
        {
            roots.resize(3);
        }

        const T pi = 4 * std::atan(T{1});

        // Cubic: x^3 + Ax^2 + Bx + C = 0
        T A = coeffs[2] / coeffs[3];
        T B = coeffs[1] / coeffs[3];
        T C = coeffs[0] / coeffs[3];

        // Substitute x = y - A/3 to eliminate quadric term:
        // x^3 + px + q = 0
        T sqA = A * A;
        T p   = T{1} / T{3} * (-T{1} / T{3} * sqA + B);
        T q = T{1} / T{2} * (T{2} / T{27} * A * sqA - T{1} / T{3} * A * B + C);

        // Use Cardano's formula.
        T cbP = p * p * p;
        T D   = q * q + cbP;

        std::size_t num{0};
        if (isZero<T>(D))
        {
            if (isZero<T>(q))
            {
                // Multiplicity 3.
                roots[0] = 0;
                num      = 1;
            }
            else
            {
                // Multiplicity 2 and 1.
                T u      = std::cbrt(-q);
                roots[0] = 2 * u;
                roots[1] = -u;
                num      = 2;
            }
        }
        else if (D < 0)
        {
            // Multiplicity 1 all.
            T phi = T{1} / T{3} * std::acos(-q / std::sqrt(-cbP));
            T t   = T{2} * std::sqrt(-p);

            const auto piBy3 = pi / static_cast<T>(3);
            roots[0]         = t * std::cos(phi);
            roots[1]         = -t * std::cos(phi + piBy3);
            roots[2]         = -t * std::cos(phi - piBy3);
            num              = 3;
        }
        else
        {
            // One real solution.
            T sqrtD = std::sqrt(D);
            T u     = std::cbrt(sqrtD - q);
            T v     = -std::cbrt(sqrtD + q);

            roots[0] = u + v;
            num      = 1;
        }

        // Resubstitute.
        T sub = T{1} / T{3} * A;

        for (std::size_t i{0}; i < num; ++i)
        {
            roots[i] -= sub;
        }

        return num;
    }

    /**
     * Solves a quartic equation of the form Ax^4 + Bx^3 + Cx^2 + Dx + E. Note
     * that the coefficients need to be passed in reverse, so {E, D, C, B, A}.
     *
     * Due to the comparison function used, roots of multiplicity greater than
     * 1 may be returned more than once.
     *
     * @param coeffs The coefficients of the quadratic equation in reverse.
     * @param roots The roots of the equation.
     * @return The number of real roots (if any).
     */
    template<typename T,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    std::size_t solveQuartic(std::vector<T> const& coeffs,
                             std::vector<T>& roots)
    {
        using atlas::core::isZero;

        assert(coeffs.size() == 5);

        auto weakIsZero = [](T x) {
            return x > -std::numeric_limits<T>::epsilon() &&
                   x < std::numeric_limits<T>::epsilon();
        };

        if (roots.empty())
        {
            roots.resize(4);
        }

        std::vector<T> c(4);

        // Quartic: x^4 + Ax^3 + Bx^2 + Cx + D = 0.
        T A = coeffs[3] / coeffs[4];
        T B = coeffs[2] / coeffs[4];
        T C = coeffs[1] / coeffs[4];
        T D = coeffs[0] / coeffs[4];

        // Substitute x = y - A / 4 to eliminate cubic term:
        // x^4 + px^2 + qx + r = 0.

        T sqA = A * A;
        T p   = -T{3} / T{8} * sqA + B;
        T q   = T{1} / T{8} * sqA * A - T{1} / T{2} * A * B + C;
        T r   = -T{3} / T{256} * sqA * sqA + T{1} / T{16} * sqA * B -
              T{1} / T{4} * A * C + D;

        std::size_t num{0};
        if (isZero<T>(r) || weakIsZero(r))
        {
            // No absolute term: y(y^3 + py + q) = 0.
            c[0] = q;
            c[1] = p;
            c[2] = 0;
            c[3] = 1;

            num          = solveCubic<T>(c, roots);
            roots[num++] = 0;
        }
        else
        {
            // Solve the resolvent cubic...
            c[0] = T{1} / T{2} * r * p - T{1} / T{8} * q * q;
            c[1] = -r;
            c[2] = -T{1} / T{2} * p;
            c[3] = 1;

            solveCubic<T>(c, roots);

            // And take the one real solution
            T z = roots[0];

            // To build two quadratics.
            T u = z * z - r;
            T v = 2 * z - p;

            if (isZero<T>(u) || weakIsZero(u))
            {
                u = 0;
            }
            else if (u > 0)
            {
                u = std::sqrt(u);
            }
            else
            {
                return 0;
            }

            if (isZero<T>(v) || weakIsZero(v))
            {
                v = 0;
            }
            else if (v > 0)
            {
                v = std::sqrt(v);
            }
            else
            {
                return 0;
            }

            c[0] = z - u;
            c[1] = (q < 0) ? -v : v;
            c[2] = 1;

            num = solveQuadratic<T>(c, roots);

            c[0] = z + u;
            c[1] = (q < 0) ? v : -v;
            c[2] = 1;

            std::vector<T> s(2, T{0});
            std::size_t tmp = num;
            num += solveQuadratic<T>(c, s);
            roots[tmp]     = s[0];
            roots[tmp + 1] = s[1];
        }

        // Resubstitute.
        T sub = T{1} / T{4} * A;

        for (std::size_t i{0}; i < num; ++i)
        {
            roots[i] -= sub;
        }

        return num;
    }
} // namespace atlas::math
