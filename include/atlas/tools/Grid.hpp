/**
 *	\file Grid.hpp
 *	\brief Defines a simple grid used for reference when modeling.
 */

#ifndef ATLAS_INCLUDE_ATLAS_TOOLS_GRID_HPP
#define ATLAS_INCLUDE_ATLAS_TOOLS_GRID_HPP

#pragma once

#include "Tools.hpp"
#include "atlas/utils/Geometry.hpp"
#include "atlas/gl/VertexArrayObject.hpp"
#include "atlas/gl/Buffer.hpp"

namespace atlas
{
    namespace tools
    {
        class Grid final : public utils::Geometry
        {
        public:
            Grid();

            void renderGeometry(atlas::math::Matrix4 const& projection,
                atlas::math::Matrix4 const& view) override;

        private:
            atlas::gl::VertexArrayObject mVao;
            atlas::gl::Buffer mBuffer;
            std::size_t mNumIndices;

        };
    }
}

#endif