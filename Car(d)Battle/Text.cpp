#include "Text.h"

Text::Text()
	:ttf(Singleton<TrueTypeFont>())
{
	set_color(glm::vec4(255, 255, 255, 255));
}

Text::Text(const Text& text)
	:ttf(text.ttf), color(text.color), mvp(text.mvp)
{
	this->text = text.text;
	font = text.font;
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
}

Text& Text::operator=(const Text& text)
{
	this->Text::Text(text);
	return *this;
}

void Text::draw()
{
	const Shader& shader = ttf.get_shader();
	shader.enable();
	shader.set("textColor", color);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glm::mat4 shift(1.0f);

	for (const char c : text)
	{
		// render glyph texture over quad
		shader.set("mvp", mvp * shift);
		shift = glm::translate(shift, glm::vec3(ttf.get_char_width(font, c), 0.0f, 0.0f));
		glBindTexture(GL_TEXTURE_2D, ttf.get_tex_id(font, c));
		glBindVertexArray(ttf.get_VAO(font, c));

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glDisable(GL_TEXTURE_2D);
}

void Text::draw(const glm::mat4 & mvp)
{
	set_mvp(mvp);
	draw();
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

void Text::set_mvp(const glm::mat4& mvp)
{
	this->mvp = mvp;
}

int Text::get_width()
{
	int width = 0;
	for (const char c : text)
		width += ttf.get_char_width(font, c);
	return width;
}

void Text::set_font(const std::string& font)
{
	std::string path;
	size_t requiredSize;
	getenv_s(&requiredSize, NULL, 0, "systemroot");

	if (requiredSize)
	{
		char* systemroot = new char[requiredSize];
		getenv_s(&requiredSize, systemroot, requiredSize, "systemroot");
		path = systemroot + std::string("\\Fonts");
		delete[] systemroot;
	}

	this->set_font(path, font);
}
