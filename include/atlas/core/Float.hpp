#pragma once

#include <cmath>
#include <cstdlib>
#include <functional>
#include <limits>
#include <type_traits>

namespace atlas::core
{
    template<typename T,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    using EpsilonFn = T (*)();

    template<typename T,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    inline constexpr bool areEqual(T a, T b)
    {
        const T scale = (std::abs(a) + std::abs(b)) / static_cast<T>(2.0);
        return std::abs(a - b) <= std::numeric_limits<T>::epsilon() * scale;
    }

    template<typename T,
             EpsilonFn<T> epsilon,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    inline constexpr bool areEqual(T a, T b)
    {
        const T scale = (std::abs(a) + std::abs(b)) / static_cast<T>(2.0);
        return std::abs(a - b) <= epsilon() * scale;
    }

    template<typename T,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    inline constexpr bool isZero(T a)
    {
        return areEqual<T>(a, static_cast<T>(0));
    }

    template<typename T,
             EpsilonFn<T> epsilon,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    inline constexpr bool isZero(T a)
    {
        return areEqual<T, epsilon>(a, static_cast<T>(0));
    }

    template<typename T,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    inline constexpr bool geq(T a, T b)
    {
        return (a > b) || areEqual<T>(a, b);
    }

    template<typename T,
             EpsilonFn<T> epsilon,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    inline constexpr bool geq(T a, T b)
    {
        return (a > b) || areEqual<T, epsilon>(a, b);
    }

    template<typename T,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    inline constexpr bool leq(T a, T b)
    {
        return (a < b) || areEqual<T>(a, b);
    }

    template<typename T,
             EpsilonFn<T> epsilon,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    inline constexpr bool leq(T a, T b)
    {
        return (a < b) || areEqual<T, epsilon>(a, b);
    }
} // namespace atlas::core
