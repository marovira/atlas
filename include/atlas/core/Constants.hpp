/**
 * \file Constants.hpp
 * \brief Defines some useful constants used throughout graphics applications.
 * These are fully templated and default to float.
 */

#ifndef ATLAS_INCLUDE_ATLAS_CORE_CONSTANTS_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_CONSTANTS_HPP

#pragma once

#include <limits>

#if defined(max)
#undef max
#endif

#if defined(min)
#undef min
#endif

namespace atlas
{
    namespace core
    {
        /**
         * Returns the defined epsilon value for that data type.
         * 
         * @tparam The numerical data type.
         */
        template <typename GenType = float>
        constexpr GenType epsilon()
        {
            return std::numeric_limits<GenType>::epsilon();
        }

        /**
         * Returns NaN for that data type.
         */
        template <typename GenType = float>
        constexpr typename std::enable_if<
            std::numeric_limits<GenType>::has_quiet_NaN, GenType>::type
        NaN()
        {
            return std::numeric_limits<GenType>::quiet_NaN();
        }

        /**
         * Returns the value of positive infinity.
         * \note
         * This function is disabled for types that do not have a
         * valid representation of positive infinity. For the maximum value
         * that a type can hold, see max().
         * 
         * @tparam The numerical data type.
         */
        template <typename GenType = float>
        constexpr typename std::enable_if<
            std::numeric_limits<GenType>::has_infinity, GenType>::type
        infinity()
        {
            return std::numeric_limits<GenType>::infinity();
        }

        /**
         * Returns the value of negative infinity.
         * \note
         * This function is disabled for types tat do not have a valid
         * representation of negative infinity. For the minimum value that a
         * type can hold, see min().
         * 
         * @tparam The numerical data type.
         */
        template <typename GenType = float>
        constexpr typename std::enable_if<
            std::numeric_limits<GenType>::has_infinity, GenType>::type
            negInfinity()
        {
            return GenType(-1) * infinity<GenType>();
        }

        /**
         * Returns the maximum value that the type can hold. Not to be confused
         * with infinity. For further details, see infinity().
         * 
         * @tparam The numerical data type.
         */
        template <typename GenType>
        constexpr GenType max()
        {
            return std::numeric_limits<GenType>::max();
        }

        /**
         * Returns the minimum value that the type can hold. Not to be confused
         * with negative infinity. For further details, see negInfinity().
         * 
         * @tparam The numerical data type.
         */
        template <typename GenType>
        constexpr GenType min()
        {
            return std::numeric_limits<GenType>::lowest();
        }
    }
}

#endif