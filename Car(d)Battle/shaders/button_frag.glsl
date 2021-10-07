#version 330 core

in vec2 _tex_coords;

out vec4 FragColor;

uniform sampler2D tex_id;
uniform float tex_shift;

void main()
{
	FragColor = texture(tex_id, vec2(_tex_coords.x, _tex_coords.y + tex_shift));
}
