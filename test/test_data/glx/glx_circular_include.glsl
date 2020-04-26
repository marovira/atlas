#version 450 core

#include "circular_include_a.glsl"

layout(location = 0) in vec4 pos;

void main()
{
    gl_Position = vec4(pos.xyz, 1.0);
}
