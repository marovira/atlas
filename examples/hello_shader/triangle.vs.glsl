#version 450 core

#include "UniformLocations.glsl"

layout(location = VERTEX_ATTRIB_LOCATION) in vec3 position;
layout(location = COLOUR_ATTRIB_LOCATION) in vec3 colour;

out vec3 vertexColour;

void main()
{
    gl_Position = vec4(position, 1.0);
    vertexColour = colour;
}
