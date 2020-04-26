#version 450 core

layout(location = 0) in vec4 pos;

// Some comment line.
layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

// Another comment line.
uniform mat4 model;

void main()
{
    // More comments.
    gl_Position = projection * view * model * vec4(pos.xyz, 1.0);
}
