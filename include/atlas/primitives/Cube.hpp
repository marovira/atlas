#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_CUBE_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_CUBE_HPP

#pragma once

#include "Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        /**
         *	\class Cube
         *	\brief Generates a solid cube.
         *	
         *	Generates a cube centered at the origin with the given
         *	dimensions. This also sets the normals for shading.
         *	
         *	\note
         *	Although the name of the class may be a bit misleading, it
         *	actually allows you to create a box of any dimensions you
         *	prefer. It is called cube for simplicity.
         */
        class Cube : public Primitive
        {
        public:
            /**
             *	Constructor for the cube.
             *	
             *	\param[in] width The width of the cube.
             *	\param[in] height The height of the cube.
             *	\param[in] depth The depth of the cube.
             */
            Cube(float width = 1.0f, float height = 1.0f, 
                float depth = 1.0f);

            /**
             *	Standard copy constructor.
             */
            Cube(Cube const& c);

            /**
             *	Standard destructor.
             */
            ~Cube();

        private:
            struct CubeImpl;
            std::unique_ptr<CubeImpl> mImpl;
        };
    }
}

#endif