/**
 * \file Utils.hpp
 * \brief Main header for utils module.
 * 
 * This header defines the macro for using its namespace. All headers
 * within this module include this header. It also provides forward
 * declarations for all of its classes.
 */

#ifndef ATLAS_INCLUDE_ATLAS_UTILS_UTILS_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_UTILS_HPP

#pragma once

#include <memory>

namespace atlas
{
    namespace utils
    {
        class Camera;
        class FPSCounter;
        class Geometry;
        class Scene;
        struct WindowSettings;

        class BBox;
        
        template <class Type, class BVType>
        class BVNode;

        template <class Type, class BVType>
        class BVH;

        template <class Type, class BVType>
        using BVNodePtr = std::shared_ptr<BVNode<Type, BVType>>;
    }
}

#endif