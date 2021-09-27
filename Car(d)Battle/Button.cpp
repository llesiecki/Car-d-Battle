#include "Button.h"

Button::Button()
	:pressed(), highlight(), texture(nullptr), local(),
	pos(), size(), m(), p(), text(), shader(), transform()
{
	GLfloat vertices_data[] = {	// pos.x, pos.y, tex.x, tex.y
		// button box with the left part of the texture:
		0.0f, 1.0f,	0.0f, 1.0f,	// upper left
		0.0f, 0.0f,	0.0f, 0.0f,	// lower left
		1.0f, 0.0f,	0.5f, 0.0f,	// lower right
		1.0f, 1.0f, 0.5f, 1.0f,	// upper right

		// button box with the right part of the texture:
		0.0f, 1.0f,	0.5f, 1.0f,	// upper left
		0.0f, 0.0f,	0.5f, 0.0f,	// lower left
		1.0f, 0.0f,	1.0f, 0.0f,	// lower right
		1.0f, 1.0f, 1.0f, 1.0f,	// upper right
	};

	GLubyte indices[] = {
		// drawing the box with the left part of the texture:
		0, 1, 2,	// first triangle
		2, 3, 0,	// second triangle

		// drawing the box with the right part of the texture:
		4, 5, 6,	// first triangle
		6, 7, 4,	// second triangle
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

Button::~Button()
{
	if (texture)
		delete texture;
}

void Button::racalculate_transform()
{
	transform = p * m * local;
}

void Button::draw() const
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());
	shader.enable();
	shader.set("tex_id", 0);
	shader.set("transform", transform);
	glBindVertexArray(vao);

	if(pressed) // draw the box with the left part of the texture:
		// 2 * 3 -> 2 triangles, 3 vertexes each
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE,
			reinterpret_cast<void*>(1 * 2 * 3 * sizeof(GLubyte)));
	else // draw the box with the right part of the texture:
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE,
			static_cast<void*>(0));

	glDisable(GL_TEXTURE_2D);
}
