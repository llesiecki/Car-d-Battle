#version 330 core

layout (location = 0) in vec2 verts;
layout (location = 1) in vec2 tex_coords;

uniform mat4 transform;

out vec2 _tex_coords;

void main()
{
    gl_Position = transform * vec4(verts, 0.0, 1.0);
    _tex_coords = tex_coords;
}