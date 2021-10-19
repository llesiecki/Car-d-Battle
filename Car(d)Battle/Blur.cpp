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

Blur::~Blur()
{
	glDeleteTextures(1, &frame_tex);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Blur::set_size(glm::ivec2 size)
{
	this->size = size;

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, frame_tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB,
		GL_UNSIGNED_BYTE, static_cast<void*>(0));
}

void Blur::draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frame_tex);

	shader.enable();
	shader.set("tex_id", 0);

	for (int i = 0; i < 10; i++)
	{
		// The below line is commented out, because it generates
		// the GL_INVALID_OPERATION error. Most likely it's caused
		// because we try to read from a render buffer object here.
		// The code works without it somehow.
		// TODO: Use texture as color attachment instead.
		
		//glReadBuffer(GL_COLOR_ATTACHMENT0);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, size.x, size.y);
		shader.set("direction", static_cast<GLboolean>(i % 2));

		glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D, frame_tex);
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE,
			static_cast<void*>(0));
	}
}
