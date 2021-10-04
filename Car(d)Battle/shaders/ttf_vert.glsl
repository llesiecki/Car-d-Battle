#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

uniform mat4 mvp;

out vec2 TexCoords;

void main()
{
    gl_Position = mvp * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
