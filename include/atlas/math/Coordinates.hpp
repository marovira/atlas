/**
 *	\file Coordinates.hpp
 *	\brief Defines functions to convert to and from other coordinate systems.
 */
#ifndef ATLAS_INCLUDE_ATLAS_MATH_COORDINATES_HPP
#define ATLAS_INCLUDE_ATLAS_MATH_COORDINATES_HPP

#pragma once

#include "Math.hpp"
#include "atlas/core/Float.hpp"

namespace atlas
{
    namespace math
    {
        /**
         *	Converts the given Point in cartesian coordinates to its
         *	equivalent in spherical coordinates. 
         *	
         *	\param[in] p The point in cartesian coordinates.
         *	\return The converted point in spherical coordinates.
         */
        inline Point cartesianToSpherical(Point const& p)
        {
            float r = glm::length(p);
            float theta = glm::atan(p.y / p.x);
            float phi = glm::acos(p.z / r);
            return Point(r, theta, phi);
        }

        /**
         *	Converts the given Point in spherical coordinates to its
         *	equivalent in cartesian coordinates.
         *	
         *	\note
         *	The function assumes that the Point has the following coordinates:
         *	\f[
         *	(r, \theta, \phi)
         *	\f]
         *	
         *	\param[in] p The point in spherical coordinates.
         *	\return The converted point in spherical coordinates.
         */
        inline Point sphericalToCartesian(Point const& p)
        {
            float x = p.x * glm::cos(p.y) * glm::sin(p.z);
            float y = p.x * glm::sin(p.y) * glm::sin(p.z);
            float z = p.r * glm::cos(p.z);

            return Point(x, y, z);
        }

        /**
         *	Converts the given Point in cartesian coordinates to its
         *	equivalent in cylindrical coordinates.
         *	
         *	\param[in] p The point in cartesian coordinates.
         *	\return The converted point in cylindrical coordinates.
         */
        inline Point cartesianToCylindrical(Point const& p)
        {
            float r = glm::length(Point2(p));
            float theta = glm::atan(p.y / p.x);
            return Point(r, theta, p.z);
        }

        /**
         *	Converts the given Point in cylindrical coordinates to its
         *	equivalent in cylindrical coordinates.
         *	
         *	\note
         *	The function assumes that the Point has the following coordinates:
         *	\f[
         *	(r, \theta, z)
         *	\f]
         *	
         *	\param[in] p The point in cylindrical coordinates.
         *	\return The converted point in cartesian coordinates.
         */
        inline Point cylindricalToCartesian(Point const& p)
        {
            float x = p.x * glm::cos(p.y);
            float y = p.x * glm::sin(p.y);
            return Point(x, y, p.z);
        }
    }
}

#endif