/**
 *	\file VertexArrayObject.hpp
 *	\brief Defines a wrapper for OpenGL vertex array objects.
 */

#ifndef ATLAS_INCLUDE_ATLAS_GL_VERTEX_ARRAY_OBJECT_HPP
#define ATLAS_INCLUDE_ATLAS_GL_VERTEX_ARRAY_OBJECT_HPP

#pragma once

#include "GL.hpp"

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
        class VertexArrayObject : public Object
        {
        public:
            /**
             * Empty constructor. This generates the vao object.
             */
            VertexArrayObject();

            /**
             * Move constructor. 
             * In order to prevent vaos from being accidentally deleted,
             * only move semantics are enabled.
             * 
             * \param[in] rhs The vao to move.
             */
            VertexArrayObject(VertexArrayObject&& rhs);

            /**
             * Destructor. This will destroy the vao object along with the
             * any vertex attribute array entries that haven't been manually
             * disabled.
             */
            ~VertexArrayObject();

            /**
             * Move assignment operator.
             * 
             * \param[in] rhs The vao to move.
             */
            VertexArrayObject& operator=(VertexArrayObject&& rhs);

            /**
             * Binds the vao. 
             */
            void bindVertexArray();

            /**
             * Unbinds the vao by binding a null (0) object instead.
             */
            void unBindVertexArray();

            /**
             * Enables vertex attribute array specified by index.
             * 
             * In addition, the index is stored in an array so it can be
             * automatically disabled when the vao is destroyed.
             * 
             * \param[in] index The index of the generic vertex attribute to
             * be enabled.
             */
            void enableVertexAttribArray(GLuint index);

            /**
             * Disables vertex attribute array specified by index. This also
             * removes the index from the list of active attributes.
             * 
             * \param[in] index The index of the generic vertex attribute to
             * be disabled.
             */
            void disableVertexAttribArray(GLuint index);

            /**
             * Checks whether the specified vertex attribute array is enabled
             * or not. 
             * 
             * \param[in] index The index of the generic vertex attribute to
             * be checked.
             * 
             * \return True if the index is active, false otherwise.
             */
            bool isVertexAttribArrayEnabled(GLuint index);

            /**
             * Returns the handle for the vao.
             * 
             * \return The vao handle.
             */
            GLuint getHandle() const override;

        private:
            struct VertexArrayImpl;
            std::unique_ptr<VertexArrayImpl> mImpl;

        };
    }
}

#endif