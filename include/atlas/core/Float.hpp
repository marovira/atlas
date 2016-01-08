/**
 * \file Float.hpp
 * \brief Defines functions to deal with comparing floating point types
 * for equality and mixed comparisons (\c <= and \c >=). 
 * \note Whenever this documentation uses the word "equal", it is understood
 * that the two floating point numbers are not equal in the mathematical sense.
 * Rather, they are sufficiently close to each other to be considered equal.
 */
#ifndef ATLAS_INCLUDE_ATLAS_CORE_FLOAT_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_FLOAT_HPP

#pragma once

#include "Core.hpp"
#include "Constants.hpp"

namespace atlas
{
    namespace core
    {
        /**
         * Compares the two given floating point numbers for equality in a way
         * that prevents errors from accumulating. The algorithm is the 
         * following:
         * \f[
         *  |a - b| \leq \epsilon * ( |a| + |b| ) / 2
         *  \f]
         *  The result of this evaluation is the value returned by the 
         *  function.
         *  
         *  @tparam The floating point data type to use.
         *  @param[in] a The first number to compare.
         *  @param[in] b The second number to compare.
         *  @return The result of evaluating the expression shown above.
         */
        template <typename GenType = float>
        inline bool areEqual(GenType a, GenType b)
        {
            const GenType scale = (std::abs(a) + std::abs(b)) / GenType(2.0);
            return (std::abs(a - b) <= epsilon<GenType>() * scale);
        }

        /**
         * Compares the given floating point number to 0. This is equivalent 
         * to <tt>areEqual(a, 0)</tt>.
         * 
         * @tparam The floating point data type to use.
         * @param[in] a The number to compare to 0.
         * @return Whether the number is equal to 0.
         */
        template <typename GenType = float>
        inline bool isZero(GenType a)
        {
            return areEqual<GenType>(a, GenType(0.0));
        }

        /**
         * Compares the given floating point number to 1. This is implemented
         * by using <tt>areEqual(a, 1)</tt>.
         * 
         * @tparam The floating point data type to use.
         * @param[in] a The number to compare to 1.
         * @return Whether the number is equal to 1.
         */
        template <typename GenType = float>
        inline bool isOne(GenType a)
        {
            return areEqual<GenType>(a, GenType(1.0));
        }

        /**
         * Tests whether \f$ a \geq b \f$. This is equivalent to evaluating:
         * <tt> (a > b) || areEqual(a, b) </tt>
         * 
         * @tparam The floating point data type to use.
         * @param[in] a The first number to compare.
         * @param[in] b The second number to compare.
         * @return The result of evaluating the expression shown above.
         */
        template <typename GenType = float>
        inline bool geq(GenType a, GenType b)
        {
            return (a > b) || areEqual<GenType>(a, b);
        }

        /**
         * Tests whether \f$ a \leq b \f$. This is equivalent to evaluating:
         * <tt> (a < b) || areEqual(a, b) </tt>
         * 
         * @tparam The floating point data type to use.
         * @param[in] a The first number to compare.
         * @param[in] b The second number to compare.
         * @return The result of evaluating the expression shown above.
         */
        template <typename GenType = float>
        inline bool leq(GenType a, GenType b)
        {
            return (a < b) || areEqual<GenType>(a, b);
        }
    }
}

#endif