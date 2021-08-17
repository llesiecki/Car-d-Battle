#include "Text.h"

Text::Text()
{
	set_color(glm::vec4(255, 255, 255, 255));
	VAO = 0;
	VBO = 0;
}

Text::Text(const std::string& text)
{
	this->Text::Text();
	set_text(text);
}

Text::Text(const std::string& text, glm::vec4 color)
{
	this->Text::Text(text);
	set_color(color);
}

Text::~Text()
{
	if (VAO)
		glDeleteVertexArrays(1, &VAO);

	if (VBO)
		glDeleteBuffers(1, &VBO);
}

void Text::draw()
{
}

void Text::set_text(const std::string& text)
{
	this->text = text;
}

void Text::set_color(glm::vec4 color)
{
	this->color = color;
}

void Text::set_color(glm::vec3 color)
{
	set_color(glm::vec4(color, this->color.w));
}

void Text::set_color(glm::vec3 color, GLfloat opacity)
{
	set_color(glm::vec4(color, opacity));
}

void Text::set_opacity(GLfloat opacity)
{
	this->color.w = opacity;
}
