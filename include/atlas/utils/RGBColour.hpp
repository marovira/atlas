/**
 *	\file RGBColour.hpp
 *	\brief Provides a typedef for colours and common functions.
 */
#ifndef ATLAS_INCLUDE_ATLAS_UTILS_RGBCOLOUR_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_RGBCOLOUR_HPP

#pragma once

#include "Utils.hpp"

#include <glm/vec3.hpp>
#include <glm/exponential.hpp>

#include <algorithm>

namespace atlas
{
    namespace utils
    {
        /**
         *	\typedef RGBColour
         *	An easy way of making code more readable instead of directly
         *	using GLM's vec3.
         */
        typedef glm::vec3 RGBColour;

        /**
         *	This function takes each element of the RGBColour and 
         *	elevates it to the given power.
         *	
         *	\param[in] c The RGBColour to compute.
         *	\param[in] po The exponent to which each element is elevated.
         *	\return The result of elevating each element to the given power.
         */
        inline RGBColour powc(RGBColour const& c, float po)
        {
            return RGBColour(
                glm::pow(c.r, po),
                glm::pow(c.g, po),
                glm::pow(c.b, po));
        }

        /**
         *	Returns a white colour.
         */
        inline RGBColour white()
        {
            return RGBColour(1.0f);
        }

        /**
         *	Returns a black colour.
         */
        inline RGBColour black()
        {
            return RGBColour(0.0f);
        }

        /**
         *	Used to clamp down RGB values between 0 and 1 by taking
         *	the maximum value and normalizing according to that value. 
         *	In other words, given an RGBColour triple \f$(r, g, b)\f$, then
         *	this function does the following:
         *	\f[
         *	\frac{(r, g, b)}{\max(r, g, b)}
         *	\f]
         *	provided the maximum is greater than 1, otherwise the colour
         *	is left unchanged.
         *	
         *	\param[in] c The colour to clamp to 1.
         *	\return The clamped colour.
         */
        inline RGBColour maxToOne(RGBColour const& c)
        {
            float max = std::max(c.r, std::max(c.g, c.b));
            return (max > 1.0f) ? c / max : c;
        }

        /**
         *	Clamps each element to the maximum each colour can hold (1 in this
         *	case). So if an element of the given colour exceeds 1, then it is
         *	clamped down.
         *	
         *	\param[in] rawColour The colour to clamp
         *	\return The clamped colour.
         */
        inline RGBColour clampToColour(RGBColour const& rawColour)
        {
            return RGBColour(
                (rawColour.r > 1.0f) ? 1.0f : rawColour.r,
                (rawColour.g > 1.0f) ? 1.0f : rawColour.g,
                (rawColour.b > 1.0f) ? 1.0f : rawColour.b);
        }
    }
}

#endif