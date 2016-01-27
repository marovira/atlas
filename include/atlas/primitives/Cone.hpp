#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_CONE_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_CONE_HPP

#pragma once

#include "Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        /**
         *	\class Cone
         *	\brief Generates a polygonal cone.
         *	
         *	Creates a cone with the given radius and height. The two
         *	subdivision parameters determine the number of "slices" the cone
         *	will have. The x subdivisions will set the number of vertical 
         *	slices, while the y subdivisions will set the horizontal ones.
         *	
         *	\note
         *	The cone is created using cylindrical coordinates. In math, the
         *	axes are flipped in relation to OpenGL. To be more specific, in
         *	math z is up, while in OpenGL y is up. As a result the cone is
         *	generated flat on the xz-plane with its tip pointing towards
         *	positive z.
         */
        class Cone : public Primitive
        {
        public:
            /**
             *	Constructor for the cone.
             *	
             *	\param[in] radius The radius of the cone.
             *	\param[in] height The height of the cone.
             *	\param[in] subDivsX The number of vertical "slices".
             *	\param[in] subDivsY The number of horizontal "slices".
             */
            Cone(float radius = 1.0f, float height = 1.0f, int subDivsX = 20,
                int subDivsY = 20);

            /**
             *	Standard copy constructor.
             */
            Cone(Cone const& c);

            /**
             *	Standard destructor.
             */
            ~Cone();

        private:
            struct ConeImpl;
            std::unique_ptr<ConeImpl> mImpl;
        };
    }
}


#endif