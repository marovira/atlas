#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_PRIMITIVE_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_PRIMITIVE_HPP

#pragma once

#include "Primitives.hpp"
#include "atlas/gl/GL.hpp"
#include "atlas/gl/VertexArrayObject.hpp"
#include "atlas/gl/Buffer.hpp"
#include "atlas/math/Vector.hpp"

#include <vector>

namespace atlas
{
    namespace primitives
    {
        /**
         *	\class Primitive
         *	\brief Defines an interface for polygonal primitives.
         *	
         *	This class provides a way of integrating procedurally created
         *	polygonal primitives into your scene. This class essentially
         *	handles the following tasks for rendering a primitive:
         *	
         *	- Create the arrays for vertices, normals, and indices.
         *	- Bind these arrays into OpenGL buffers.
         *	- Draw the buffers.
         *	
         *	In order to use this interface, simply subclass it and add the
         *	code to fill in the buffers inside the new constructor. The
         *	three protected functions are there if you require any specific
         *	operations to occur when binding, unbidning, and drawing the 
         *	buffers.
         *	
         *	It is important to note that this class does <b>not</b> handle
         *	shader operations. This is left to the user. Also, since the 
         *	primitives all have vertex and normal buffers, your shaders
         *	should have the following layout:
         *	\code
         *	#version 330
         *	layout(location = 0) vec3 vPosition;
         *	layout(location = 1) vec3 vNormal;
         *	...
         *	\endcode
         *	
         *	Of course, the actual location can be changed by overriding the
         *	default bindBuffers function.
         */
        class Primitive
        {
        public:
            /**
             *	Standard empty constructor.
             */
            Primitive();

            /**
             *	Standard default copy constructor.
             */
            Primitive(Primitive const& prim) = default;

            /**
             *	Standard virtual destructor.
             */
            virtual ~Primitive();

            /**
             *	This function creates the OpenGL buffers and binds their 
             *	data using the provided vector, normal, and index arrays.
             *	
             *	\note
             *	This function does not populate the vertex, normal, and index
             *	arrays. Please do this inside the constructor for your derived
             *	class.
             */
            virtual void createBuffers();

            /**
             *	Draws out the arrays. It essentially performs the following
             *	function calls (in this order):
             *	
             *	\code
             *	bindBuffers();
             *	drawBuffers();
             *	unBindBuffers();
             *	\endcode
             *	
             *	Please be aware of this order if you choose to override 
             *	this function.
             */
            virtual void drawPrimitive();

        protected:
            /**
             *	\var mVao
             *	The handle for the vertex array object (one per primitive).
             */
            atlas::gl::VertexArrayObject mVao;

            /**
             *	\var mDataBuffer
             *	The handle for the data buffer (data is interlaced from 
             *  mVertices and mNormals).
             */
            atlas::gl::Buffer mDataBuffer;

            /**
             *	\var mIndexBuffer
             *	The handle for the index buffer (data comes from mIndices).
             */
            atlas::gl::Buffer mIndexBuffer;

            /**
             *	\var mVertices
             *	Contains the vertex data for the primitive.
             */
            std::vector<atlas::math::Point> mVertices;

            /**
             *	\var mNormals
             *	Contains the normal data for the primitive.
             */
            std::vector<atlas::math::Normal> mNormals;

            /**
             *	\var mIndices
             *	Contains the indices that represent the triangles for the
             *	primitive.
             */
            std::vector<GLuint> mIndices;
        };
    }
}

#endif
