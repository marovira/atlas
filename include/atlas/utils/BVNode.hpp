#ifndef ATLAS_INCLUDE_ATLAS_UTILS_BV_NODE_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_BV_NODE_HPP

#pragma once

#include "Utils.hpp"
#include "atlas/math/Math.hpp"

#include <vector>

namespace atlas
{
    namespace utils
    {
        enum class NodeType : int
        {
            Node = 0,
            Leaf
        };

        template <class Type, class BVType>
        class BVNode
        {
            BVNode()
            { }

            virtual ~BVNode()
            { }

            virtual BVNodePtr<NodeType, BVType> cloneShared() = 0;

            virtual NodeType getType() const = 0;
            virtual std::vector<Type> visit(
                atlas::math::Point const& p) const = 0;
            virtual std::vector<Type> visit(BVType const& bv) const = 0;
            virtual BVType getVolume() const = 0;
        };
    }
}

#endif