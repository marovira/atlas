/**
 *	\file MayaCamera.hpp
 *	\brief Defines a camera that behaves similar to the one used in 
 *	Autodesk Maya.
 */

#ifndef ATLAS_INCLUDE_ATLAS_TOOLS_MAYA_CAMERA_HPP
#define ATLAS_INCLUDE_ATLAS_TOOLS_MAYA_CAMERA_HPP

#pragma once

#include "Tools.hpp"
#include "atlas/utils/Camera.hpp"

#include <memory>

namespace atlas
{
    namespace tools
    {
        enum class MayaMovements : int
        {
            None = 0,
            Tumble,
            Track,
            Dolly
        };

        class MayaCamera final : public utils::Camera
        {
        public:
            MayaCamera();
            ~MayaCamera();

            void mouseDown(atlas::math::Point2 const& point) override;
            void mouseMove(atlas::math::Point2 const& point) override;
            void mouseUp() override;
            void mouseScroll(atlas::math::Point2 const& offset) override;
            void resetCamera() override;

            void setMovement(MayaMovements movement);

            atlas::math::Point getCameraPosition() const override;
            atlas::math::Matrix4 getCameraMatrix() const override;
            float getCameraFOV() const override;

        private:
            struct MayaCameraImpl;
            std::unique_ptr<MayaCameraImpl> mImpl;
        };
    }
}


#endif