/**
 *	\file Buffer.hpp
 *	\brief Defines a wrapper for OpenGL buffer objects.
 */

#ifndef ATLAS_INCLUDE_ATLAS_GL_BUFFER_HPP
#define ATLAS_INCLUDE_ATLAS_GL_BUFFER_HPP

#pragma once

#include "GL.hpp"

#include <memory>

namespace atlas
{
    namespace gl
    {
        /**
         * Computes a pointer offset into a buffer that holds data of the 
         * specified type.
         * 
         * \tparam GenType The data type that is contained in the buffer.
         * \param[in] offset The offset into the buffer.
         * 
         * \return A pointer into the buffer that is offset by the provided 
         * amount.
         */
        template <typename GenType>
        constexpr void* bufferOffset(std::size_t offset)
        {
            return (void*)(offset * sizeof(GenType));
        }

        /**
         * Computes the stride that separates one object from another in a 
         * buffer that holds data of the specified type.
         * 
         * \tparam GenType The data type that is contained in the buffer.
         * \param[in] step The distance between objects.
         * 
         * \return The distance (in bytes) between objects.
         */
        template <typename GenType>
        constexpr GLsizei stride(std::size_t step)
        {
            return (GLsizei)(step * sizeof(GenType));
        }

        /**
         * 
         * Computes the size of the specified number of objects.
         * 
         * \tparam GenType The data type for which we are computing the size.
         * \param[in] num The number of elements.
         * 
         * \return THe size (in bytes) of the elements.
         */
        template <typename GenType>
        constexpr std::size_t size(std::size_t num)
        {
            return stride<GenType>(num);
        }

        /**
         *	\class Buffer
         *	\brief A container for OpenGL buffer objects.
         *	
         *	This class encapsulates the handle for the buffer object, 
         *	thereby providing simplified access to the most common buffer
         *	functions.
         */
        class Buffer : public Object
        {
        public:
            /**
             * Standard empty constructor. Note that this does not generate a
             * buffer.
             */
            Buffer();

            /**
             *	Creates a buffer with the specified target.
             *	
             *	\param[in] target The type of buffer. This is used in the rest
             *	of the functions.
             */
            Buffer(GLenum target);

            /**
             * Move constructor. 
             * In order to prevent buffers from being accidentally deleted, 
             * only move semantics are enabled.
             * 
             * \param[in] rhs The buffer to move.
             */
            Buffer(Buffer&& rhs);

            /**
             * Move assignment operator.
             * 
             * \param[in] rhs The buffer to move.
             * 
             * \return The moved buffer.
             */
            Buffer& operator=(Buffer&& rhs);

            /**
             *	This destroys the buffer handle that was created and sets
             *	the handle back to 0.
             */
            ~Buffer();

            /**
             *	Used in tandem with the \c Buffer() constructor. 
             *	
             *	\param[in] target The type of buffer. This is used in the rest
             *	of the functions.
             */
            void genBuffer(GLenum target);

            /**
             *	Binds the buffer handle contained by the Buffer object to the
             *	target specified upon construction or \c genBuffer.
             */
            void bindBuffer() const;

            /**
             *	Unbinds the buffer handle contained by the Buffer object by
             *	binding a null (0) buffer to the specified target.
             */
            void unBindBuffer() const;

            /**
             *	Creates a new data store for the currently bound buffer
             *	object and populates it with the provided data.
             *	
             *	\param[in] size The size in bytes of the buffer object's new
             *	data store.
             *	\param[in] data The pointer to the data that will be copied
             *	into the data store for initialization or NULL if no data is
             *	copied.
             *	\param[in] usage The usage pattern for the data store.
             */
            void bufferData(GLsizeiptr size, const GLvoid* data, 
                GLenum usage) const;

            /**
             *	Redefines some (or all) of the data store for the currently
             *	bound buffer object.
             *	
             *	\param[in] offset The offset into the buffer object's data 
             *	store where the replacement will begin, measured in bytes.
             *	\param[in] size The size in bytes of the data store region
             *	being replaced.
             *	\param[in] data The pointer to the data that will
             *	be copied into the data store.
             */
            void bufferSubData(GLintptr offset, GLsizeiptr size, 
                const GLvoid* data) const;

            /**
             *	Fills a buffer object's data store with a fixed value.
             *	
             *	\param[in] internalFormat The internal format with which the
             *	data will be stored in the buffer object.
             *	\param[in] format THe format of the data in memory addressed
             *	by data.
             *	\param[in] type The type of the data in memory addressed by
             *	data.
             *	\param[in] data The pointer to the data that will be
             *	replicated in the buffer's data store.
             */
            void clearBufferData(GLenum internalFormat, GLenum format,
                GLenum type, const void* data) const;

            /**
             *	Fill all or part of a buffer object's data store with a 
             *	fixed value.
             *	
             *	\param[in] internalFormat The internal format with which the
             *	data will be stored in the buffer object.
             *	\param[in] offset The offset into the buffer object's data
             *	store, measured in bytes.
             *  \param[in] size The size in bytes of the data store region to
             *  be replaced.
             *	\param[in] format THe format of the data in memory addressed
             *	by data.
             *	\param[in] type The type of the data in memory addressed by
             *	data.
             *	\param[in] data The pointer to the data that will be
             *	replicated in the buffer's data store.
             */
            void clearBufferSubData(GLenum internalFormat, GLintptr offset,
                GLintptr size, GLenum format, GLenum type, 
                const void* data) const;

            /**
             *	Returns a subset of the buffer object's data store.
             *	
             *	\param[in] offset The offset into the buffer object's data
             *	store, measured in bytes.
             *	\param[in] size The size of the data being returned, in bytes.
             *	\param[in] data The pointer to the location where the buffer
             *	object data is returned.
             */
            void getBufferSubData(GLintptr offset, GLsizeiptr size, 
                GLvoid* data) const;

            /**
             *	Map all of a buffer object's data store into the client's
             *	address space.
             *	
             *	\param[in] access The access policy,.
             */
            void* mapBuffer(GLenum access) const;

            /**
             * Map all or part of a buffer object's data store to the client's
             * address space.
             * 
             * \param[in] offset Specifies the starting offset within the buffer
             * of the range to be mapped.
             * \param[in] length Specifies the length of the range to be mapped.
             * \param[in] access Specifies the combination of access flags 
             * indicating the desired access to the mapped range.
             */
            void* mapBufferRange(GLintptr offset, GLsizeiptr length,
                GLbitfield access) const;

            /**
             *	Release the mapping of a buffer object's data store into the
             *	client's address space.
             */
            GLboolean unMapBuffer() const;

            /**
             *	Define an array of generic vertex attribute data.
             *	
             *	\param[in] index The index of the generic vertex attribute.
             *	\param[in] size The number of components per generic vertex
             *	attribute.
             *	\param[in] type The data type of each component in the array.
             *	\param[in] normalized Whether the fixed-point data values
             *	should be normalized.
             *	\param[in] stride The byte offset between generic vertex 
             *	attributes.
             *	\param[in] pointer The offset of the first component of the 
             *	first generic vertex attribute in the array.
             */
            void vertexAttribPointer(GLuint index, GLint size,
                GLenum type, GLboolean normalized, GLsizei stride,
                const GLvoid* pointer) const;

            /**
             * Bind a range within a buffer object to an indexed buffer 
             * target.
             * 
             * \param[in] index The binding point within the array specified
             * by the target of the Buffer.
             * \param[in] offset The starting offset in machine units into the 
             * Buffer object.
             * \param[in] size The amount of data in machine units that can
             * be read from the buffer object.
             */
            void bindBufferRange(GLuint index, GLintptr offset, 
                GLsizeiptr size) const;

            /**
             * Bind a buffer object to an indexed buffer target.
             * 
             * \param[in] index The binding point within the array specified
             * by the target of the Buffer.
             */
            void bindBufferBase(GLuint index) const;

            /**
             * Returns the buffer handle.
             * 
             * \return The buffer handle.
             */
            GLuint getHandle() const override;

        private:
            struct BufferImpl;
            std::unique_ptr<BufferImpl> mImpl;

        };
    }
}

#endif