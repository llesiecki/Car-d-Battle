#include "Dimmer.h"

Dimmer::Dimmer()
	:mvp(1.0f), size(0.0f, 0.0f)
{
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);

	GLfloat vertices_data[] = { //pos.x, pos.y, pos.z
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	GLubyte indices[] = {
		0, 1, 2,   // first triangle
		2, 3, 0    // second triangle
	};

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data), vertices_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// attrib 0 - vertex coords
	// attrib num, qty of buffer items, type, normalize?, size of a single vertex, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<void*>(0));
	glEnableVertexAttribArray(0);

	opacity = 0.5f;

	shader.load("shaders\\dimmer_vert.glsl", GL_VERTEX_SHADER);
	shader.load("shaders\\dimmer_frag.glsl", GL_FRAGMENT_SHADER);
	shader.link();
}

Dimmer::~Dimmer()
{
	if (vao != 0)
		glDeleteVertexArrays(1, &vao);

	if (vbo != 0)
		glDeleteBuffers(1, &vbo);

	if (ebo != 0)
		glDeleteBuffers(1, &ebo);
}

void Dimmer::draw()
{
	shader.enable();
	shader.set("opacity", opacity);
	shader.set("mvp", mvp * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1 }));
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE, static_cast<void*>(0));
}

void Dimmer::set_mvp(const glm::mat4& mvp)
{
	this->mvp = mvp;
}

void Dimmer::set_opacity(GLfloat opacity)
{
	this->opacity = opacity / 100.0f;
}

void Dimmer::set_size(glm::vec2 size)
{
	this->size = size;
}
