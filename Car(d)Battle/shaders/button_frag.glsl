#version 330 core

in vec2 _tex_coords;

out vec4 FragColor;

uniform sampler2D tex_id;

void main()
{
	FragColor = texture(tex_id, _tex_coords);
}
