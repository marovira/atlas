/**
 * \file Enum.hpp
 * \brief Defines a way of using scoped enums as array indices.
 */

#ifndef ATLAS_INCLUDE_ATLAS_CORE_ENUM_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_ENUM_HPP

#pragma once

#include <type_traits>

namespace atlas
{
    namespace core
    {
        /**
         * Converts the given enum into it's underlying type so it can be used
         * in other operations (e.g. an enum class that uses int as an
         * underlying value can then be used as an index).
         * 
         * \tparam EnumType The type of the enum. <b>Must</b> be an enum class.
         * \param[in] value The enum value to convert.
         * 
         * \return The result of converting the enum value to its underlying 
         * type.
         */
        template <typename EnumType>
        constexpr typename std::underlying_type<EnumType>::type
            enumToUnderlyingType(EnumType value)
        {
            using underType = typename std::underlying_type<EnumType>::type;
            return static_cast<underType>(value);
        }
    }
}

#endif