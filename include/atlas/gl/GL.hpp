/**
 * \file GL.hpp
 * \brief Main header for the GL module.
 * 
 * This header essentially encapsulates the inclusion of the headers
 * for OpenGL depending on the platform. All the user has to do is add the
 * following line:
 * \code
 * #include <atlas/gl/GL.hpp>
 * \endcode
 * and all OpenGL functionality will be available. It also defines the
 * macro for its namespace. All headers within this module include this
 * header.
 * 
 * \warning
 * Atlas does <b>not</b> support OpenGL 2 or any extension that is not in the
 * core profile specified by OpenGL. The profile can be seen
 * <a href="https://www.opengl.org/registry/api/GL/glcorearb.h">here</a> 
 */

#ifndef ATLAS_INCLUDE_ATLAS_GL_GL_HPP
#define ATLAS_INCLUDE_ATLAS_GL_GL_HPP

#pragma once

#include <GL/gl3w.h>

namespace atlas
{
    namespace gl
    {
        /**
         * \class Object
         * \brief Base class for all OpenGL objects.
         * 
         * Provides the standard interface that all objects must implement.
         */
        class Object
        {
        public:
            /**
             * Returns the handle of the objet.
             * 
             * \return The object's handle.
             */
            virtual GLuint getHandle() const = 0;
        };

        struct ShaderUnit;

        class Shader;
        class Texture;
        class Buffer;
        class VertexArrayObject;
        class Mesh;
    }
}

#endif
