#include "Blur.h"

Blur::Blur()
{
	frame_tex = 0;
	vao = 0;
	vbo = 0;
	ebo = 0;

	glGenTextures(1, &frame_tex);

	set_size({ 1280, 720 });

	GLfloat vertices_data[] = {	//pos.x, pos.y, tex.x, tex.y
	-1.0f, 1.0f,	0.0f, 1.0f,	//upper left
	-1.0f, -1.0f,	0.0f, 0.0f,	//lower left
	1.0f, -1.0f,	1.0f, 0.0f,	//lower right
	1.0f, 1.0f,		1.0f, 1.0f,	//upper right
	};

	GLubyte indices[] = {
		0, 1, 2,	// first triangle
		2, 3, 0,	// second triangle
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data), vertices_data,
		GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		GL_STATIC_DRAW);

	//attrib 0 - vertex coords
	glVertexAttribPointer(
		0, // attrib num
		2, // qty of buffer items
		GL_FLOAT, // type
		GL_FALSE, // normalize?
		4 * sizeof(GLfloat), // stride
		static_cast<void*>(0) // offset
	);
	glEnableVertexAttribArray(0);

	//attrib 1 - tex coords
	glVertexAttribPointer(
		1, // attrib num
		2, // qty of buffer items
		GL_FLOAT, // type
		GL_FALSE, // normalize?
		4 * sizeof(GLfloat), // stride
		reinterpret_cast<void*>(2 * sizeof(GLfloat)) // offset
	);
	glEnableVertexAttribArray(1);

	shader.load("shaders\\blur_vert.glsl", GL_VERTEX_SHADER);
	shader.load("shaders\\blur_frag.glsl", GL_FRAGMENT_SHADER);
	shader.link();
}
