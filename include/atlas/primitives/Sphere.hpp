#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_SPHERE_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_SPHERE_HPP

#pragma once

#include "Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        /**
         *	\class Sphere
         *	\brief Generates a polygonal sphere.
         *	
         *	Creates a sphere with the given radius centered at the origin.
         *	The two subdivision parameters determine the number of slices 
         *	that the sphere will have. The x subdivisions set the number 
         *	of vertical slices, while the y subdivisions set the 
         *	horizontal ones.
         */
        class Sphere : public Primitive
        {
        public:
            /**
             *	Constructor for the sphere.
             *	
             *	\param[in] radius The radius of the sphere.
             *	\param[in] subDivsX The number of vertical slices.
             *	\param[in] subDivsY The number of horizontal slices.
             */
            Sphere(float radius = 1.0f, int subDivsX = 20, int subDivsY = 20);

            /**
             *	Standard copy constructor.
             */
            Sphere(Sphere const& s);

            /**
             *	Standard destructor.
             */
            ~Sphere();

        private:
            struct SphereImpl;
            std::unique_ptr<SphereImpl> mImpl;
        };
    }
}

#endif