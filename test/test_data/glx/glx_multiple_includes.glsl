#version 450 core

#include "uniform_bindings.glsl"

layout(location = VERTEX_LOCATION) in vec4 pos;

#include "uniform_matrices.glsl"

void main()
{
    gl_Position = projection * view * model * vec4(pos.xyz, 1.0);
}
