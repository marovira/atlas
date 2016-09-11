/**
 *	\file Ray.hpp
 *	\brief Defines a ray class.
 */

#ifndef ATLAS_INCLUDE_ATLAS_MATH_RAY_HPP
#define ATLAS_INCLUDE_ATLAS_MATH_RAY_HPP

#pragma once

#include "Math.hpp"

namespace atlas
{
    namespace math
    {
        /**
         *	\class Ray
         *	\brief Defines a mathematical ray with an origin and a direction.
         *	
         *	This class can be used for ray casting, or any other operation
         *	that requires rays. The <tt> () </tt> operator has been overloaded
         *	as a more intuitive way of evaluating the ray at the given value.
         *	
         *	\note
         *	The direction given to the ray need not be normalized, and the class
         *	does not normalize directions.
         */
        struct Ray
        {
            /**
             *	Standard constructor. Initializes the origin of the ray to 
             *	\f$ (0, 0, 0 \f$ and the direction to \f$ (0, 1, 0) \f$.
             */
            Ray() :
                o(0.0f),
                d(0.0f, 1.0f, 0.0f)
            { }

            /**
             *	Constructs a ray with the given origin and direction.
             *	
             *	@param[in] origin The ray origin.
             *	@param[in] dir The direction of the ray.
             */
            Ray(Point const& origin, Vector const& dir) :
                o(origin),
                d(dir)
            { }

            /**
             *	Default copy constructor.
             */
            Ray(Ray const& ray) = default;

            /**
             *	Default destructor.
             */
            ~Ray()
            { }

            /**
             *	Default assignment operator.
             *	
             *	@param[in] rhs The ray to copy.
             */
            Ray& operator=(Ray const& rhs) = default;

            /**
             *	Overrides the evaluation operator to compute the following
             *	equation
             *	\f[
             *	\mathbf{o} + t * \mathbf{d}
             *	\f]
             *	
             *	@param[in] t The parameter at which to evaluate the ray.
             *	@return The result of evaluating the above expression.
             */
            Point operator()(float t) const
            {
                return o + (t * d);
            }

            /**
             *	Standard comparison operator.
             *	
             *	@param[in] rhs The ray to compare with.
             *	@return Whether the two rays are equal or not.
             */
            bool operator==(Ray const& rhs)
            {
                return (o == rhs.o) && (d == rhs.d);
            }

            /**
             *	Standard inequality operator.
             *	
             *	@param[in] rhs The ray to compare with.
             *	@return Whether the two rays are different or not.
             */
            bool operator!=(Ray const& rhs)
            {
                return !((*this) == rhs);
            }

            /**
             *	\var o
             *	The origin of the ray.
             */
            Point o;

            /**
             *	\var d
             *	The direction of the ray.
             */
            Vector d;
        };

    }
}

#endif