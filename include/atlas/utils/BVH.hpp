#ifndef ATLAS_INCLUDE_ATLAS_UTILS_BVH_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_BVH_HPP

#pragma once

#include "Utils.hpp"

#include "atlas/math/Math.hpp"

#include <memory>
#include <vector>

namespace atlas
{
    namespace utils
    {
        template <class NodeType, class BVType>
        class BVH
        {
        public:
            BVH() :
                mRoot(std::make_shared<BVNode<NodeType, BVType>>())
            { }

            virtual ~BVH()
            { }

            virtual void construct(std::vector<NodeType> const& leaves) = 0;

            virtual std::vector<NodeType> 
                traverse(atlas::math::Point const& p) const = 0;

            virtual std::vector<NodeType> traverse(BVType const& bv) const = 0;

            virtual BVType getGlobalVolume() const = 0;

        protected:
            std::shared_ptr<BVNode<NodeType>> mRoot;

        };
    }
}

#endif