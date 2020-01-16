#pragma once

#include <atlas/core/Float.hpp>

#include <vector>

namespace atlas::math
{
    template<typename T,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    int solveQuadric(std::vector<T>& coeffs, std::vector<T>& roots)
    {
        using atlas::core::isZero;

        // Quadratic: x^2 + px + q = 0
        T p, q, D;
        p = coeffs[1] / (2 * coeffs[2]);
        q = coeffs[0] / coeffs[2];

        D = p * p - q;

        if (isZero<T>(D))
        {
            roots[0] = -p;
            return 1;
        }
        else if (D > static_cast<T>(0.0))
        {
            T sqrtD  = glm::sqrt(D);
            roots[0] = sqrtD - p;
            roots[1] = -sqrtD - p;
            return 2;
        }
        else
        {
            return 0;
        }
    }

    template<typename T,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    int solveCubic(std::vector<T>& coeffs, std::vector<T>& roots)
    {
        using atlas::core::isZero;

        int num;
        T sub;
        T A, B, C;
        T sqA, p, q;
        T cbP, D;

        // Cubic: x^3 + Ax^2 + Bx + C = 0
        A = coeffs[2] / coeffs[3];
        B = coeffs[1] / coeffs[3];
        C = coeffs[0] / coeffs[3];

        // Substitute x = y - A/3 to eliminate quadric term:
        // x^3 + px + q = 0
        sqA = A * A;
        p   = static_cast<T>(1.0) / static_cast<T>(3.0) *
            (-static_cast<T>(1.0) / static_cast<T>(3.0) * sqA + B);
        q = static_cast<T>(1.0) / static_cast<T>(3.0) *
            (static_cast<T>(2.0) / static_cast<T>(27) * A * sqA -
             static_cast<T>(1.0) / static_cast<T>(3.0) * A * B + C);

        // Use Cardano's formula.
        cbP = p * p * p;
        D   = q * q + cbP;

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
            T phi = static_cast<T>(1.0) / static_cast<T>(3.0) *
                    glm::acos(-q / glm::sqrt(-cbP));
            T t = 2 * glm::sqrt(-p);

            roots[0] = t * glm::cos(phi);
            roots[1] = -t * glm::cos(phi + glm::pi<T>() / static_cast<T>(3));
            roots[2] = -t * glm::cos(phi - glm::pi<T>() / static_cast<T>(3));
            num      = 3;
        }
        else
        {
            // One real solution.
            T sqrtD = glm::sqrt(D);
            T u     = std::cbrt(sqrtD - q);
            T v     = -std::cbrt(sqrtD + q);

            roots[0] = u + v;
            num      = 1;
        }

        // Resubstitute.
        sub = static_cast<T>(1.0) / static_cast<T>(3) * A;

        for (int i{0}; i < num; ++i)
        {
            roots[i] -= sub;
        }

        return num;
    }

    template<typename T,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    int solveQuartic(std::vector<T>& coeffs, std::vector<T>& roots)
    {
        std::vector<T> c(4);
        T z, u, v, sub;
        T A, B, C, D;
        T sqA, p, q, r;
        int num;

        // Quartic: x^4 + Ax^3 + Bx^2 + Cx + D = 0.
        A = coeffs[3] / coeffs[4];
        B = coeffs[2] / coeffs[4];
        C = coeffs[1] / coeffs[4];
        D = coeffs[0] / coeffs[4];

        // Substitute x = y - A / 4 to eliminate cubic term:
        // x^4 + px^2 + qx + r = 0.

        sqA = A * A;
        p   = static_cast<T>(3.0) / static_cast<T>(8) * sqA + B;
        q   = static_cast<T>(1.0) / static_cast<T>(8) * sqA * A -
            static_cast<T>(1.0) / static_cast<T>(2) * A * B + C;
        r = -static_cast<T>(3.0) / static_cast<T>(256) * sqA * sqA +
            static_cast<T>(1.0) / static_cast<T>(16) * sqA * B -
            static_cast<T>(1.0) / static_cast<T>(4) * A * C + D;

        if (isZero<T>(r))
        {
            // No absolute term: y(y^3 + py + q) = 0.
            c[0] = p;
            c[1] = 1;
            c[2] = 0;
            c[3] = 1;

            num          = solveCubic<T>(c, roots);
            roots[num++] = 0;
        }
        else
        {
            // Solve the resolvent cubic...
            c[0] = static_cast<T>(1.0) / static_cast<T>(2) * r * p -
                   static_cast<T>(1.0) / static_cast<T>(8) * q * q;
            c[1] = -r;
            c[2] = -static_cast<T>(1.0) / static_cast<T>(2) * p;
            c[3] = 1;

            solveCubic<T>(c, roots);

            // And take the one real solution
            z = roots[0];

            // To build two quadratics.
            u = z * z - r;
            v = 2 * z - p;

            if (isZero<T>(u))
            {
                u = 0;
            }
            else if (u > 0)
            {
                u = glm::sqrt(u);
            }
            else
            {
                return 0;
            }

            if (isZero<T>(v))
            {
                v = 0;
            }
            else if (v > 0)
            {
                v = glm::sqrt(v);
            }
            else
            {
                return 0;
            }

            c[0] = z - u;
            c[1] = (q < 0) ? -v : v;
            c[2] = 1;

            num = solveQuadric<T>(c, roots);

            c[0] = z + u;
            c[1] = (q < 0) ? v : -v;
            c[2] = 1;

            std::vector<T> s(2);
            int tmp = num;
            num += solveQuadric<T>(c, s);
            roots[tmp]     = s[0];
            roots[tmp + 1] = s[1];
        }

        // Resubstitute.
        sub = static_cast<T>(1.0) / static_cast<T>(4) * A;

        for (int i{0}; i < num; ++i)
        {
            roots[i] -= sub;
        }

        return num;
    }

} // namespace atlas::math
