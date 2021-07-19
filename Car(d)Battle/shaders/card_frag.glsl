#version 330 core

in vec2 aTexCoords;

out vec4 FragColor;

uniform sampler2D TexID;

void main()
{
	FragColor = texture(TexID, aTexCoords);
}
