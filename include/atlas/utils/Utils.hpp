/**
 * \file Utils.hpp
 * \brief Main header for Utils module.
 * 
 * This header defines the macro for using its namespace. All headers
 * within this module include this header. It also provides forward
 * declarations for all of its classes.
 */
#ifndef ATLAS_INCLUDE_ATLAS_UTILS_UTILS_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_UTILS_HPP

#pragma once

#include "atlas/Atlas.hpp"

#ifdef USING_ATLAS_UTILS_NS
#undef USING_ATLAS_UTILS_NS
#endif

/**
 *	\def USING_ATLAS_UTILS_NS
 *	Shortened version for using the utils namespace.
 */
#define USING_ATLAS_UTILS_NS using namespace atlas::utils

namespace atlas
{
    namespace utils
    {
        class Film;
        class Geometry;
        class Scene;
        struct Time;
    }
}

#endif