#ifndef ATLAS_INCLUDE_ATLAS_GL_GL_HPP
#define ATLAS_INCLUDE_ATLAS_GL_GL_HPP

#pragma once

#include "atlas/Atlas.hpp"
#include "atlas/core/Platform.hpp"

#if !defined(ATLAS_PLATFORM_APPLE)
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#ifdef USING_ATLAS_GL_NS
#undef USING_ATLAS_GL_NS
#endif

#define USING_ATLAS_GL_NS using namespace atlas::gl

namespace atlas
{
    namespace gl
    {
        class Shader;
        class Texture;
    }
}

#endif
