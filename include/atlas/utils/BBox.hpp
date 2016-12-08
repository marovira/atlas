#ifndef ATLAS_INCLUDE_ATLAS_UTILS_BBOX_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_BBOX_HPP

#pragma once

#include "Utils.hpp"

#include "atlas/math/Math.hpp"

namespace atlas
{
    namespace utils
    {
        class BBox
        {
        public:
            BBox();
            BBox(atlas::math::Point const& p);
            BBox(atlas::math::Point const& p1, atlas::math::Point const& p2);
            ~BBox() = default;

            bool overlaps(BBox const& b) const;
            bool inside(atlas::math::Point const& p) const;
            void expand(float delta);

            friend BBox join(BBox const& b1, BBox const& b2);

            atlas::math::Point pMin, pMax;
        };
    }
}

#endif