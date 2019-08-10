#pragma once

#include <type_traits>

namespace atlas::core
{
    template<typename T, typename = std::enable_if<std::is_enum<T>::value>>
    constexpr typename std::underlying_type<T>::type
    enumToUnderlyingType(T value)
    {
        using UnderType = typename std::underlying_type<T>::type;
        return static_cast<UnderType>(value);
    }

    template<typename T,
             typename = std::enable_if<
                 std::is_enum<T>::value &&
                 std::is_unsigned<std::underlying_type<T>::type>::value>>
    constexpr T operator~(T a)
    {
        using UnderType = typename std::underlying_type<T>::type;
        UnderType var   = ~enumToUnderlyingType(a);
        return static_cast<T>(var);
    }

    template<typename T,
             typename = std::enable_if<
                 std::is_enum<T>::value &&
                 std::is_unsigned<std::underlying_type<T>::type>::value>>
    constexpr T operator|(T a, T b)
    {
        using UnderType = typename std::underlying_type<T>::type;
        UnderType uA    = enumToUnderlyingType(a);
        UnderType uB    = enumToUnderlyingType(b);
        return static_cast<T>(uA | uB);
    }

    template<typename T,
             typename = std::enable_if<
                 std::is_enum<T>::value &&
                 std::is_unsigned<std::underlying_type<T>::type>::value>>
    constexpr T operator&(T a, T b)
    {
        using UnderType = typename std::underlying_type<T>::type;
        UnderType uA    = enumToUnderlyingType(a);
        UnderType uB    = enumToUnderlyingType(b);
        return static_cast<T>(uA & uB);
    }

    template<typename T,
             typename = std::enable_if<
                 std::is_enum<T>::value &&
                 std::is_unsigned<std::underlying_type<T>::type>::value>>
    constexpr T operator^(T a, T b)
    {
        using UnderType = typename std::underlying_type<T>::type;
        UnderType uA    = enumToUnderlyingType(a);
        UnderType uB    = enumToUnderlyingType(b);
        return static_cast<T>(uA ^ uB);
    }

} // namespace atlas::core
