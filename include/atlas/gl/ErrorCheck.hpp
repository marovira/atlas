#ifndef ATLAS_INCLUDE_ATLAS_GL_ERROR_CHECK_HPP
#define ATLAS_INCLUDE_ATLAS_GL_ERROR_CHECK_HPP

#pragma once

#include "atlas/core/Macros.hpp"
#include "GL.hpp"

namespace atlas
{
    namespace gl
    {
        static void checkGLErrors();
    }
}

#ifdef ATLAS_DEBUG
#define GL_ERROR_CHECK() atlas::gl::checkGLErrors()
#else
#define GL_ERROR_CHECK()
#endif

#endif