#ifndef UNIFORM_MATRICES_GLSL
#define UNIFORM_MATRICES_GLSL

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

#endif
