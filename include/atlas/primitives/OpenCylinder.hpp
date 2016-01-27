#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_OPEN_CYLINDER_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_OPEN_CYLINDER_HPP

#pragma once

#include "Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        class OpenCylinder : public Primitive
        {
        public:
            OpenCylinder(float radius = 1.0f, float height = 1.0f,
                int subDivsX = 20, int subDivsY = 20);
            OpenCylinder(OpenCylinder const& oc);
            ~OpenCylinder();

        private:
            struct OpenCylinderImpl;
            std::unique_ptr<OpenCylinderImpl> mImpl;
        };
    }
}

#endif