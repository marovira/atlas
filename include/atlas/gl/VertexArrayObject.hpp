#ifndef ATLAS_INCLUDE_ATLAS_GL_VERTEX_ARRAY_OBJECT_HPP
#define ATLAS_INCLUDE_ATLAS_GL_VERTEX_ARRAY_OBJECT_HPP

#pragma once

#include "GL.hpp"
#include "Buffer.hpp"

#include <memory>
#include <vector>

namespace atlas
{
    namespace gl
    {
        /**
         * \class VertexArrayObject
         * \brief Wrapper for OpenGL vertex array objects.
         * 
         * This class wraps around the most common vao operations in OpenGL.
         * It also provides automatic debugging for errors from OpenGL. In
         * addition it contains a list of all the vertex attribute array
         * indeces that are used when this vao is bound. That way they 
         * can be set once and the class will take care of deleting them
         * when the destructor is called.
         */
        class VertexArrayObject
        {
        public:
            /**
             * Empty constructor. This generates the vao object and is
             * equivalent to 
             * 
             * \code{.cpp}
             * glGenVertexArrays(1, &handle);
             * \endcode
             */
            VertexArrayObject();

            /**
             * Copy constructor.
             * 
             * \note
             * The constructor only copies the handle along with the list
             * of vertex attribute array entires. It <b>does not</b> create a 
             * new vao from the old one.
             */
            VertexArrayObject(VertexArrayObject const& vao);

            /**
             * Destructor. This will destroy the vao object along with the
             * any vertex attribute array entries that haven't been manually
             * disabled.
             */
            ~VertexArrayObject();

            /**
             * Binds the vao. This is equivalent to 
             * 
             * \code{.cpp}
             * glBindVertexArray(handle);
             * \endcode
             */
            void bindVertexArray();

            /**
             * Unbinds the vao. This is equivalent to
             * 
             * \code{.cpp}
             * glBindVertexArray(0);
             * \endcode
             */
            void unBindVertexArray();

            /**
             * Enables vertex attribute array specified by index. This is
             * equivalent to
             * 
             * \code{.cpp}
             * glEnableVertexAttribArray(index);
             * \endcode
             * 
             * In addition, the index is stored in an array so it can be
             * automatically disabled when the vao is destroyed.
             * 
             * \param[in] index The index of the generic vertex attribute to
             * be enabled.
             */
            void enableVertexAttribArray(GLuint index);

            /**
             * Disables vertex attribute array specified by index. This is
             * equivalent to
             * 
             * \code{.cpp}
             * glDisableVertexAttribArray(index);
             * \endcode
             * 
             * This also removes the index from the array.
             * 
             * \param[in] index The index of the generic vertex attribute to
             * be disabled.
             */
            void disableVertexAttribArray(GLuint index);

        private:
            struct VertexArrayImpl;
            std::unique_ptr<VertexArrayImpl> mImpl;

        };
    }
}

#endif