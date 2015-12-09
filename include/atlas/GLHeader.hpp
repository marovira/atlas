#ifndef APOLLO_INCLUDE_ATLAS_GL_HEADER_HPP
#define APOLLO_INCLUDE_ATLAS_GL_HEADER_HPP

#pragma once

#include "Platform.hpp"

#if !defined(APOLLO_PLATFORM_APPLE)
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#endif
