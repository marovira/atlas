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
        /**
         * \class Grid
         * \brief A simple reference grid used for modelling and animation
         * projects. Please be advised that the rendering code for this
         * class assumes an OpenGL version of 4.3 or above.
         */
        class Grid final : public utils::Geometry
        {
        public:
            /**
             * Empty constructor.
             */
            Grid();

            /**
             * The grid class provides its own rendering code and its own 
             * shaders. All that is required are the projection and view
             * matrices.
             * 
             * \param[in] projection The projection matrix of the scene.
             * \param[in] view The view matrix of the scene.
             */
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