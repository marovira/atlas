/**
 * \file Core.hpp
 * \brief Main header for the Core module. This header defines the macro
 * for using its namespace, as well as forward-declares any classes that the
 * module contains. All headers that define classes within this module 
 * include this header.
 */

#ifndef ATLAS_INCLUDE_ATLAS_CORE_CORE_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_CORE_HPP

#pragma once

#include "atlas/Atlas.hpp"

#ifdef USING_ATLAS_CORE_NS
#undef USING_ATLAS_CORE_NS
#endif

/**
 * \def USING_ATLAS_CORE_NS
 * Shortened version for using the core namespace.
 */
#define USING_ATLAS_CORE_NS using namespace atlas::core

namespace atlas
{
    namespace core
    {
        template <class GenType>
        class Timer;
    }
}

#endif