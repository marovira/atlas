#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_OPEN_CYLINDER_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_OPEN_CYLINDER_HPP

#pragma once

#include "Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        /**
         *	\class OpenCylinder
         *	\brief Constructs a polygonal cylinder.
         *	
         *	Creates a cylinder with the given radius and height. The two
         *	subdivision parameters determine the number of slices in which
         *	the cylinder will be split into. The x subdivisions denote 
         *	vertical slices while y subdivisions denote horizontal ones.
         *	
         *	Please be aware that this is a mathematical cylinder, which
         *	means that it represents a section of an infinite object that
         *	has no caps. In order to create a closed cylinder, you can add
         *	the caps with disks.
         *	
         *	\note
         *	This cylinder uses cylindrical coordinates to layout the points, 
         *	which assumes that z is up (as opposed to y as in OpenGL). This 
         *	means that the cylinder will be crossing the xy plane and will
         *	have the z axis as its central axis.
         */
        class OpenCylinder : public Primitive
        {
        public:
            /**
             *	Constructor for the cylinder.
             *	
             *	\param[in] radius The radius of the cylinder.
             *	\param[in] height The height of the cylinder.
             *	\param[in] subDivsX The vertical slices of the cylinder.
             *	\param[in] subDivsY The horizontal slices of the cylinder.
             */
            OpenCylinder(float radius = 1.0f, float height = 1.0f,
                int subDivsX = 20, int subDivsY = 20);

            /**
             *	Standard copy constructor.
             */
            OpenCylinder(OpenCylinder const& oc);

            /**
             *	Standard destructor.
             */
            ~OpenCylinder();

        private:
            struct OpenCylinderImpl;
            std::unique_ptr<OpenCylinderImpl> mImpl;
        };
    }
}

#endif