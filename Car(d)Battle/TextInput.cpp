#include "TextInput.h"

TextInput::TextInput()
	:texture(nullptr), scale(),
	pos(), size(), text(), shader(), transform(), proj(),
	kb(nullptr), cursor_pos(nullptr), scaled_size(), translate()
{
	GLfloat vertices_data[] = {	// pos.x, pos.y, tex.x, tex.y
		// button rectangle with the bottom part of the texture,
		// tex coords can be shifted with a uniform
		0.0f, 1.0f,	0.0f, 0.25f,	// upper left
		0.0f, 0.0f,	0.0f, 0.0f,		// lower left
		1.0f, 0.0f,	1.0f, 0.0f,		// lower right
		1.0f, 1.0f, 1.0f, 0.25f,	// upper right
	};

	GLubyte indices[] = {
		// drawing the button rectangle:
		0, 1, 2,	// first triangle
		2, 3, 0,	// second triangle
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data),
		vertices_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);

	// attrib 0 - vertex coords:
	glVertexAttribPointer(
		0, // attrib num
		2, // qty of buffer items
		GL_FLOAT, // type
		GL_FALSE, // normalize?
		4 * sizeof(GLfloat), // stride
		static_cast<void*>(0) // offset
	);
	glEnableVertexAttribArray(0);

	// attrib 1 - tex coords:
	glVertexAttribPointer(
		1, // attrib num
		2, // qty of buffer items
		GL_FLOAT, // type
		GL_FALSE, // normalize?
		4 * sizeof(GLfloat), // stride
		reinterpret_cast<void*>(2 * sizeof(GLfloat)) // offset
	);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader.load("shaders\\button_vert.glsl", GL_VERTEX_SHADER);
	shader.load("shaders\\button_frag.glsl", GL_FRAGMENT_SHADER);
	shader.link();
}

TextInput::~TextInput()
{
	if (texture)
		delete texture;
}
