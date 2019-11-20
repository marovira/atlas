#version 450 core

#include "nested_include.glsl"

layout(location = VERTEX_LOCATION) in vec4 pos;

void main()
{
    gl_Position = projection * view * model * vec4(pos.xyz, 1.0);
}
