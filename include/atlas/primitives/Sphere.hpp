#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_SPHERE_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_SPHERE_HPP

#pragma once

#include "Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        class Sphere : public Primitive
        {
        public:
            Sphere(float radius = 1.0f, int subDivsX = 20, int subDivsY = 20);
            Sphere(Sphere const& s);
            ~Sphere();

        private:
            struct SphereImpl;
            std::unique_ptr<SphereImpl> mImpl;
        };
    }
}

#endif