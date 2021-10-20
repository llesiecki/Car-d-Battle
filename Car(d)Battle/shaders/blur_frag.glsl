#version 330 core
out vec4 FragColor;

in vec2 _tex_coords;

uniform sampler2D tex_id;
uniform bool direction;

void main()
{             
	vec2 texel_size = 1.0 / textureSize(tex_id, 0);
	const float weights[5] = float[](
		0.227,
		0.195,
		0.122,
		0.054,
		0.016
	);
	vec2 step_size = vec2(0.0, 0.0);
	vec2 offset = vec2(0.0, 0.0);

	if(direction)
		step_size.x = texel_size.x;
	else
		step_size.y = texel_size.y;

	vec3 result = texture(tex_id, _tex_coords).rgb * weights[0];

	for(int i = 1; i < 5; ++i)
	{
		offset += step_size;
		result += texture(tex_id, _tex_coords + offset).rgb * weights[i];
		result += texture(tex_id, _tex_coords - offset).rgb * weights[i];
	}

	FragColor = vec4(result, 1.0);
}
