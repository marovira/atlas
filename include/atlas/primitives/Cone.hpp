#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_CONE_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_CONE_HPP

#pragma once

#include "Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        class Cone : public Primitive
        {
        public:
            Cone(float radius = 1.0f, float height = 1.0f, int subDivsX = 20,
                int subDivsY = 20);
            Cone(Cone const& c);
            ~Cone();

        private:
            struct ConeImpl;
            std::unique_ptr<ConeImpl> mImpl;
        };
    }
}


#endif