#include "TrueTypeFont.h"

TrueTypeFont::TrueTypeFont()
	:ft()
{
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

	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		throw std::string("ERROR::FREETYPE: Could not init FreeType Library");
	}
	
	FT_Face face;
	
	if (FT_New_Face(ft, (path + "\\" + name).c_str(), 0, &face))
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
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x,
			0,
			0
		};

		fonts[name][c] = character;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

GLuint TrueTypeFont::get_VAO(const std::string& font, char c)
{
	if (fonts.find(font) == fonts.end())
		return 0; //font not present

	if(fonts[font][c].VAO)
		return fonts[font][c].VAO;

	float xpos = static_cast<float>(
		fonts[font][c].Bearing.x);
	float ypos = static_cast<float>(
		fonts[font][c].Bearing.y - fonts[font][c].Size.y);

	float w = static_cast<float>(
		get_char_width(font, c));
	float h = static_cast<float>(
		fonts[font][c].Size.y);

	float vertices[6][4] = {
		{ xpos,     ypos + h,   0.0f, 0.0f },
		{ xpos,     ypos,       0.0f, 1.0f },
		{ xpos + w, ypos,       1.0f, 1.0f },

		{ xpos,     ypos + h,   0.0f, 0.0f },
		{ xpos + w, ypos,       1.0f, 1.0f },
		{ xpos + w, ypos + h,   1.0f, 0.0f }
	};



	// update content of VBO memory

	glGenVertexArrays(1, &fonts[font][c].VAO);
	glGenBuffers(1, &fonts[font][c].VBO);
	glBindVertexArray(fonts[font][c].VAO);
	glBindBuffer(GL_ARRAY_BUFFER, fonts[font][c].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

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

const Shader& TrueTypeFont::get_shader()
{
	return shader;
}
