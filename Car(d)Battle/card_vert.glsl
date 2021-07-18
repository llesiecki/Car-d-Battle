#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 TexCoords;

uniform mat4 transform;

out vec2 aTexCoords;

void main()
{
    gl_Position = transform * vec4(Pos, 1.0);
    aTexCoords = TexCoords;
}