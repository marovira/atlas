#pragma once

#include <type_traits>

namespace atlas::core
{
    template<typename T, typename = std::enable_if<std::is_enum<T>::value>>
    constexpr typename std::underlying_type<T>::type
    enumToUnderlyingType(T value)
    {
        using underType = typename std::underlying_type<T>::type;
        return static_cast<underType>(value);
    }
} // namespace atlas::core
