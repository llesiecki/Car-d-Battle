#version 330 core
layout (location = 0) in vec2 verts;
layout (location = 1) in vec2 tex_coords;

out vec2 _tex_coords;

void main()
{
    _tex_coords = tex_coords;
    gl_Position = vec4(verts, 0.0, 1.0);
}
