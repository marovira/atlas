#version 450 core

layout(location = 0) in vec4 pos;

#include "UniformMatrices.glsl"

void main()
{
    gl_Position = projection * view * model * vec4(pos.xyz, 1.0);
}
