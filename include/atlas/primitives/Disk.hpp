#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_DISK_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_DISK_HPP

#pragma once

#include "Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        /**
         *	\class Disk
         *	\brief Generates a polygonal disk.
         *	
         *	Creates a disk with the specified radius. The number of 
         *	subdivisions determines the number of triangles the disk is
         *	split into.
         *	
         *	\note
         *	The disk is generated using cylindrical coordinates, which assumes
         *	that z is up (as opposed to y like OpenGL does), as a result the
         *	disk is generated sitting on the xy plane.
         */
        class Disk : public Primitive
        {
        public:
            /**
             *	Constructor for the disk.
             *	
             *	\param[in] radius The radius of the disk.
             *	\param[in] subDivs The number of subdivisions for the disk.
             */
            Disk(float radius = 1.0f, int subDivs = 20);

            /**
             *	Standard copy constructor.
             */
            Disk(Disk const& d);

            /**
             *	Standard destructor.
             */
            ~Disk();

        private:
            struct DiskImpl;
            std::unique_ptr<DiskImpl> mImpl;
        };
    }
}

#endif