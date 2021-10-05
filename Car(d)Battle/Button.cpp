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

	float fill_x = (size.x * 0.8f) / text.get_width();
	float fill_y = (size.y * 0.8f) / FONT_SIZE;

	float fill_min = std::min({ fill_x, fill_y });

	glm::mat4 text_scale = glm::scale(glm::mat4(1.0f), {
		fill_min,
		fill_min,
		0 });

	float width = text.get_width();

	glm::mat4 text_center = glm::translate(glm::mat4(1.0f), {
		(size.x - width * fill_min) / 2,
		(size.y / 2) - (FONT_SIZE / 5),
		0 });

	text.set_mvp(p * m * text_center * text_scale);
}

void Button::draw()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->get_id());
	shader.enable();
	shader.set("tex_id", 0);
	shader.set("transform", transform);
	glBindVertexArray(vao);

	if (pressed) // draw the box with the left part of the texture:
		// 2 * 3 -> 2 triangles, 3 vertexes each
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE,
			reinterpret_cast<void*>(1 * 2 * 3 * sizeof(GLubyte)));
	else // draw the box with the right part of the texture:
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE,
			static_cast<void*>(0));

	glDisable(GL_TEXTURE_2D);
	text.draw();
	glEnable(GL_DEPTH_TEST);
}

bool Button::is_hovered(const glm::ivec2& cursor) const
{
	return
		cursor.x > pos.x &&
		cursor.x < pos.x + size.x &&
		cursor.y > pos.y &&
		cursor.y < pos.y + size.y;
}

void Button::set_projection(const glm::mat4& projection)
{
	this->p = projection;
	racalculate_transform();
}

void Button::set_highlight(bool highlight)
{
	this->highlight = highlight;
}

void Button::set_press(bool press)
{
	this->pressed = press;
}

void Button::set_pos(const glm::ivec2& pos)
{
	this->pos = pos;
	m = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0 });
	racalculate_transform();
}

void Button::set_size(const glm::ivec2& size)
{
	this->size = size;
	local = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1 });
	racalculate_transform();
}

void Button::set_text(const std::string& text)
{
	this->text.set_text(text);
	racalculate_transform();
}

void Button::set_font(const std::string& font)
{
	this->text.set_font(font);
	racalculate_transform();
}

void Button::set_texture(const std::string& path)
{
	if (texture)
		delete texture;
	texture = new Texture(path);
	texture->load();
	texture->bind();
}
