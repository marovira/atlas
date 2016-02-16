/**
 *	\file Primitives.hpp
 *	\brief Main header for primitives module.
 *	
 *	This header defines the macro for using its namespaces. All headers
 *	within this module include this header.
 */
#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_PRIMITIVES_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_PRIMITIVES_HPP

#pragma once

#include <memory>

#ifdef USING_ATLAS_PRIMITIVES_NS
#undef USING_ATLAS_PRIMITIVES_NS
#endif

/**
 *	\def USING_ATLAS_PRIMITIVES_NS
 *	Shortened version for using the primitives namespace.
 */
#define USING_ATLAS_PRIMITIVES_NS using namespace atlas::primtivies

namespace atlas
{
    namespace primitives
    {
        class Primitive;
        class Cube;
        class Sphere;
        class Cone;
        class Disk;
        class OpenCylinder;
    }
}

#endif