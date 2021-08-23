#include "Text.h"

Text::Text()
	:ttf(Singleton<TrueTypeFont>())
{
	set_color(glm::vec4(255, 255, 255, 255));
	VAO = 0;
	VBO = 0;
}

Text::Text(const std::string& text)
	: Text::Text()
{
	set_text(text);
}

Text::Text(const std::string& text, glm::vec4 color)
	: Text::Text(text)
{
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
	const Shader& shader = ttf.get_shader();
	shader.enable();
	shader.set("textColor", color);
	//TODO: Add a possibility to set/pass the transformation matrix
	shader.set("projection", glm::mat4());
	
	glActiveTexture(GL_TEXTURE0);

	float x = 0, y = 0;

	for (const char c : text)
	{
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ttf.get_tex_id(font, c));
		glBindVertexArray(ttf.get_VAO(font, c));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		x += ttf.get_char_width(font, c);
		//TODO: Add trnsformations for shifting the characters
	}
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

void Text::set_font(const std::string& path, const std::string& font)
{
	this->font = font;
	ttf.load_font(path, font);
}

void Text::set_font(const std::string& font)
{
	this->set_font("%systemroot%/Fonts", font);
}
