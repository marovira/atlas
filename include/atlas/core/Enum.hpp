#pragma once

#include <type_traits>

namespace atlas::core
{
    template<typename T, typename = std::enable_if<std::is_enum<T>::value>>
    using UnderlyingType = typename std::underlying_type<T>::type;

    template<typename T, typename = std::enable_if<std::is_enum<T>::value>>
    constexpr typename std::underlying_type<T>::type
    enumToUnderlyingType(T value)
    {
        return static_cast<UnderlyingType<T>>(value);
    }

    template<typename T,
             typename = std::enable_if<
                 std::is_enum<T>::value &&
                 std::is_unsigned<std::underlying_type<T>::type>::value>>
    constexpr T operator~(T a)
    {
        UnderlyingType<T> var = ~enumToUnderlyingType<T>(a);
        return static_cast<T>(var);
    }

    template<typename T,
             typename = std::enable_if<
                 std::is_enum<T>::value &&
                 std::is_unsigned<std::underlying_type<T>::type>::value>>
    constexpr T operator|(T a, T b)
    {
        UnderlyingType<T> uA = enumToUnderlyingType<T>(a);
        UnderlyingType<T> uB = enumToUnderlyingType<T>(b);
        return static_cast<T>(uA | uB);
    }

    template<typename T,
             typename = std::enable_if<
                 std::is_enum<T>::value &&
                 std::is_unsigned<std::underlying_type<T>::type>::value>>
    constexpr T operator&(T a, T b)
    {
        UnderlyingType<T> uA = enumToUnderlyingType<T>(a);
        UnderlyingType<T> uB = enumToUnderlyingType<T>(b);
        return static_cast<T>(uA & uB);
    }

    template<typename T,
             typename = std::enable_if<
                 std::is_enum<T>::value &&
                 std::is_unsigned<std::underlying_type<T>::type>::value>>
    constexpr T operator^(T a, T b)
    {
        UnderlyingType<T> uA = enumToUnderlyingType<T>(a);
        UnderlyingType<T> uB = enumToUnderlyingType<T>(b);
        return static_cast<T>(uA ^ uB);
    }

} // namespace atlas::core
