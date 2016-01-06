#ifndef ATLAS_INCLUDE_ATLAS_GL_ERROR_HPP
#define ATLAS_INCLUDE_ATLAS_GL_ERROR_HPP

#pragma once

namespace atlas
{
    class GLError
    {
    public:
        static void checkGLErrors();
    };
}

#define GL_ERROR_CHECK() atlas::GLError::checkGLErrors()

#endif