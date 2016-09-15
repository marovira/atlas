/**
 *	\file ErrorCheck.hpp
 *	\brief Defines the OpenGL error callback and error handling system.
 *
 * \warning
 * Atlas assumes the existence of the OpenGL error callback function 
 * (available in OpenGL 4.3+).
 */

#ifndef ATLAS_INCLUDE_ATLAS_GL_ERROR_CHECK_HPP
#define ATLAS_INCLUDE_ATLAS_GL_ERROR_CHECK_HPP

#pragma once

#include "GL.hpp"

#define ATLAS_GL_ERROR_SOURCE_API                   0x1
#define ATLAS_GL_ERROR_SOURCE_WINDOW_SYSTEM         0x2
#define ATLAS_GL_ERROR_SOURCE_SHADER_COMPILER       0x4
#define ATLAS_GL_ERROR_SOURCE_THIRD_PARTY           0x8
#define ATLAS_GL_ERROR_SOURCE_APPLICATION           0x10
#define ATLAS_GL_ERROR_SOURCE_OTHER                 0x20
#define ATLAS_GL_ERROR_SOURCE_ALL                   0x3F
#define ATLAS_GL_ERROR_SOURCE_NONE                  0x0

#define ATLAS_GL_ERROR_TYPE_ERROR                   0x1
#define ATLAS_GL_ERROR_TYPE_DEPRECATED_BEHAVIOUR    0x2
#define ATLAS_GL_ERROR_TYPE_UNDEFINED_BEHAVIOUR     0x4
#define ATLAS_GL_ERROR_TYPE_PORTABILITY             0x8
#define ATLAS_GL_ERROR_TYPE_PERFORMANCE             0x10
#define ATLAS_GL_ERROR_TYPE_MARKER                  0x20
#define ATLAS_GL_ERROR_TYPE_PUSH_GROUP              0x40
#define ATLAS_GL_ERROR_TYPE_POP_GROUP               0x80
#define ATLAS_GL_ERROR_TYPE_OTHER                   0x100
#define ATLAS_GL_ERROR_TYPE_ALL                     0x1FF
#define ATLAS_GL_ERROR_TYPE_NONE                    0x0

#define ATLAS_GL_ERROR_SEVERITY_NOTIFICATION        0x1
#define ATLAS_GL_ERROR_SEVERITY_LOW                 0x2
#define ATLAS_GL_ERROR_SEVERITY_MEDIUM              0x4
#define ATLAS_GL_ERROR_SEVERITY_HIGH                0x8
#define ATLAS_GL_ERROR_SEVERITY_ALL                 0xF
#define ATLAS_GL_ERROR_SEVERITY_NONE                0x0

namespace atlas
{
    namespace gl
    {
        /**
         * Registers the OpenGL error callback (provided that it is available).
         * If it isn't (or this is not a debug build), an error is printed to 
         * the log.
         */
        void initializeGLError();

        /**
         * Sets which error sources are allowed to print messages using the 
         * specified bit mask.
         * 
         * \param[in] sources The error sources that can print messages.
         */
        void setGLErrorSources(GLuint sources);

        /**
         * Sets which types of error are allowed to pring messages using the
         * specified bit mask.
         * 
         * \param[in] types The error types that can print messages.
         */
        void setGLErrorTypes(GLuint types);

        /**
         * Sets which severtiy levels are allowed to print messages using the
         * specified bit mask.
         * 
         * \param[in] severity The severity level that can print messages.
         */
        void setGLErrorSeverity(GLuint severity);

        /**
         * The error callback function for OpenGL.
         * 
         * \param[in] source The source of the error message.
         * \param[in] type The type of error message.
         * \param[in] id The error message id.
         * \param[in] severity The error message severity.
         * \param[in] length The length of the error message string.
         * \param[in] message The error message.
         * \param[in] userParam Holds no meaning to OpenGL and isn't used in
         * Atlas.
         */
        void APIENTRY openGLErrorCallback(GLenum source, GLenum type,
            GLuint id, GLenum severity, GLsizei length, const GLchar* message,
            const void* userParam);
    }
}

#endif