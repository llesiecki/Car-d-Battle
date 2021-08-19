#include "TrueTypeFont.h"

TrueTypeFont::TrueTypeFont()
{
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		throw std::string("ERROR::FREETYPE: Could not init FreeType Library");
	}

	shader.load("shaders/ttf_vert.glsl", GL_VERTEX_SHADER);
	shader.load("shaders/ttf_frag.glsl", GL_FRAGMENT_SHADER);
	shader.link();
}

TrueTypeFont::~TrueTypeFont()
{
	FT_Done_FreeType(ft);
}

void TrueTypeFont::load_font(const std::string& path, const std::string& name)
{
	if (fonts.find(name) != fonts.end())
		return; //font already present

	FT_Face face;

	if (FT_New_Face(ft, path.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		throw std::string("ERROR::FREETYPE: Failed to load font");
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			std::pair<int, int>(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			std::pair<int, int>(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x,
			0,
			0
		};

		fonts[name][c] = character;
	}

	FT_Done_Face(face);
}

void TrueTypeFont::draw(const std::string& text, const std::string& font, const glm::vec3& color)
{
	shader.enable();
	shader.set("textColor", color);
	glActiveTexture(GL_TEXTURE0);

	float x = 0, y = 0;

	for (const char c : text)
	{
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, get_tex_id(font, c));
		glBindVertexArray(get_VAO(font, c));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		x += get_char_width(font, c);
		//TODO: Add trnswormations for shifting the characters
	}
}

GLuint TrueTypeFont::get_VAO(const std::string& font, char c)
{
	if (fonts.find(font) == fonts.end())
		return 0; //font not present

	if(fonts[font][c].VAO)
		return fonts[font][c].VAO;

	glGenVertexArrays(1, &fonts[font][c].VAO);
	glGenBuffers(1, &fonts[font][c].VBO);
	glBindVertexArray(fonts[font][c].VAO);
	glBindBuffer(GL_ARRAY_BUFFER, fonts[font][c].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);


	glBindTexture(GL_TEXTURE_2D, fonts[font][c].TextureID);
	// update content of VBO memory

	float xpos = fonts[font][c].Bearing.first;
	float ypos = fonts[font][c].Size.second - fonts[font][c].Bearing.second;

	float w = get_char_width(font, c);
	float h = fonts[font][c].Size.second;
	float vertices[6][4] = {
		{ xpos,     ypos + h,   0.0f, 0.0f },
		{ xpos,     ypos,       0.0f, 1.0f },
		{ xpos + w, ypos,       1.0f, 1.0f },

		{ xpos,     ypos + h,   0.0f, 0.0f },
		{ xpos + w, ypos,       1.0f, 1.0f },
		{ xpos + w, ypos + h,   1.0f, 0.0f }
	};

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return fonts[font][c].VAO;
}

GLuint TrueTypeFont::get_tex_id(const std::string& font, char c)
{
	if (fonts.find(font) == fonts.end())
		return 0; //font not present

	return fonts[font][c].TextureID;
}

int TrueTypeFont::get_char_width(const std::string& font, char c)
{
	if (fonts.find(font) == fonts.end())
		return 0; //font not present

	return fonts[font][c].Advance >> 6; // bitshift by 6 to get value in pixels (2^6 = 64);
}
