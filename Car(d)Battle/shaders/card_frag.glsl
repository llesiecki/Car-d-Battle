#version 330 core

in vec2 _tex_coords;

out vec4 FragColor;

uniform sampler2D TexID;

void main()
{
	FragColor = texture(TexID, _tex_coords);
}
