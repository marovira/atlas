#ifndef ATLAS_INCLUDE_ATLAS_GL_BUFFER_HPP
#define ATLAS_INCLUDE_ATLAS_GL_BUFFER_HPP

#pragma once

#include "GL.hpp"
#include "Buffer.hpp"

#include <memory>

/**
 *	\def BUFFER_OFFSET(offset)
 *	Used to provide the offset pointer for vertex attribute data.
 */
#define BUFFER_OFFSET(offset) (void*)(offset)

/**
 *	\def STRIDE(step, type)
 *	Used to specify the step (or stride) between elements in the vertex
 *	attribute data.
 */
#define STRIDE(step, type) step * sizeof(type)

namespace atlas
{
    namespace gl
    {
        /**
         *	\class Buffer
         *	\brief Wrapper for OpenGL buffer objects.
         *	
         *	This class wraps around the most common buffer operations in
         *	OpenGL. It also provides automatic debugging for errors from
         *	OpenGL. 
         */
        class Buffer
        {
        public:
            /**
             * Standard empty constructor. Note that this does not
             * call \c glGenBuffer.
             */
            Buffer();

            /**
             *	This constructor creates a buffer with the specified target.
             *	It is equivalent to 

             *	\code{.cpp}
             *	glGenBuffers(1, &handle);
             *	\endcode
             *	
             *	\param[in] target The type of buffer. This is used in the rest
             *	of the functions.
             */
            Buffer(GLenum target);

            /**
             *	Standard copy constructor.
             *	
             *	\note
             *	This constructor only copies the <b>buffer handle</b>. It does 
             *	not create a copy of the existing buffer.
             *	
             *	\param[in] b The buffer to copy from.
             */
            Buffer(Buffer const& b);

            /**
             *	Destructor.
             *	
             *	\note
             *	The destructor calls 
             *	\code{.cpp}
             *	glDeleteBuffers(1, &handle);
             *	\endcode
             *	
             *	and sets the handle and target to 0. Please be aware of this
             *	fact when copying buffers.
             */
            ~Buffer();

            /**
             *	Used in tandem with the \c Buffer() constructor. It is 
             *	equivalent to 
             *	
             *	\code{.cpp}
             *	glGenBuffers(1, &handle);
             *	\endcode
             *	
             *	\param[in] target The type of buffer. This is used in the rest
             *	of the functions.
             */
            void genBuffer(GLenum target);

            /**
             *	Binds the buffer handle contained by the Buffer object to the
             *	target specified upon construction or \c getBuffer.
             */
            void bindBuffer();

            /**
             *	Unbinds the buffer handle contained by the Buffer object by
             *	binding a null (0) buffer to the specified target.
             */
            void unBindBuffer();

            /**
             *	Creates a new data store for the currently bound buffer
             *	object and populates it with the provided data. This is
             *	equivalent to 
             *	
             *	\code{.cpp}
             *	glBufferData(target, size, data, range);
             *	\endcode
             *	
             *	where target is the one provided upon construction (or
             *	when the buffer was generated).
             *	
             *	\param[in] size The size in bytes of the buffer object's new
             *	data store.
             *	\param[in] data The pointer to the data that will be copied
             *	into the data store for initialization or NULL if no data is
             *	copied.
             *	\param[in] usage The usage pattern for the data store.
             */
            void bufferData(GLsizeiptr size, const GLvoid* data, 
                GLenum usage);

            /**
             *	Redefines some (or all) of the data store for the currently
             *	bound buffer object. This is equivalent to
             *	
             *	\code{.cpp}
             *	glBufferSubData(target, offset, size, data);
             *	\endcode
             *	
             *	where target is the one provided upon construction (or when
             *	the buffer was generated).
             *	
             *	\param[in] offset The offset into the buffer object's data 
             *	store where the replacement will begin, measured in bytes.
             *	\param[in] size The size in bytes of the data store region
             *	being replaced.
             *	\param[in] data The pointer to the data that will
             *	be copied into the data store.
             */
            void bufferSubData(GLintptr offset, GLsizeiptr size, 
                const GLvoid* data);

            /**
             *	Fills a buffer object's data store with a fixed value. This
             *	is equivalent to
             *	
             *	\code{.cpp}
             *	glClearBufferData(target, internalFormat, format, type, data);
             *	\endcode
             *	
             *	where target is the one provided upon construction (or when
             *	the buffer was generated).
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
                GLenum type, const void* data);

            /**
             *	Fill all or part of a buffer object's data store with a 
             *	fixed value. This is equivalent to
             *	
             *	\code{.cpp}
             *	glClearBufferSubData(target, internalFormat, offset, 
             *	size, format, type, data);
             *	\endcode
             *	
             *	where target is the one provided upon construction (or when
             *	the buffer was generated).
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
                GLintptr size, GLenum format, GLenum type, const void* data);

            /**
             *	Returns a subset of the buffer object's data store. This is
             *	equivalent to
             *	
             *	\code{.cpp}
             *	glGetBufferSubData(target, offset, size, data);
             *	\endcode
             *	
             *	where target is the one provided upon construction (or when
             *	the buffer was generated).
             *	\param[in] offset The offset into the buffer object's data
             *	store, measured in bytes.
             *	\param[in] size The size of the data being returned, in bytes.
             *	\param[in] data The pointer to the location where the buffer
             *	object data is returned.
             */
            void getBufferSubData(GLintptr offset, GLsizeiptr size, 
                GLvoid* data);

            /**
             *	Map all of a buffer object's data store into the client's
             *	address space. THis is equivalent to
             *	
             *	\code{.cpp}
             *	glMapBuffer(target, access);
             *	\endcode
             *	
             *	where target is the one provided upon construction (or when
             *	the buffer was generated).
             *	
             *	\param[in] access The access policy,.
             */
            void* mapBuffer(GLenum access);

            /**
             *	Release the mapping of a buffer object's data store into the
             *	client's address space. This is equivalent to
             *	
             *	\code{.cpp}
             *	glUnmapBuffer(target);
             *	\endcode
             *	
             *	where target is the one provided upon construction (or when
             *	the buffer was generated).
             */
            GLboolean unMapBuffer();

            /**
             *	Define an array of generic vertex attribute data. This is
             *	equivalent to
             *	
             *	\code{.cpp}
             *	glVertexAttribPointer(index, size, type, normalized, stride,
             *	pointer);
             *	\endcode
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
                const GLvoid* pointer);

        private:
            struct BufferImpl;
            std::unique_ptr<BufferImpl> mImpl;

        };
    }
}

#endif