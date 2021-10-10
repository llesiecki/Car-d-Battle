#include "TrueTypeFont.h"

TrueTypeFont::TrueTypeFont()
{
	shader.load("shaders/ttf_vert.glsl", GL_VERTEX_SHADER);
	shader.load("shaders/ttf_frag.glsl", GL_FRAGMENT_SHADER);
	shader.link();
}

TrueTypeFont::~TrueTypeFont()
{
}

void TrueTypeFont::load_font(const std::string& path, const std::string& name)
{
	if (fonts.find(name) != fonts.end())
		return; //font already present

	FT_Library ft;

	if (FT_Init_FreeType(&ft))
	{
		std::cout <<
			"ERROR::FREETYPE: Could not init FreeType Library"
			<< std::endl;
		throw std::string("ERROR::FREETYPE: Could not init FreeType Library");
	}
	
	FT_Face face;
	
	if (FT_New_Face(ft, (path + "\\" + name).c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		throw std::string("ERROR::FREETYPE: Failed to load font");
	}

	FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);

	GLint initial_unpack_alignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &initial_unpack_alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout
				<< "ERROR::FREETYTPE: Failed to load Glyph"
				<< std::endl;
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
		// store character
		fonts[name][c] = {
			texture,
			0,
			0,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			// bitshift by 6 to get value in pixels (2^6 = 64)
			face->glyph->advance.x >> 6
		};
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, initial_unpack_alignment);

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
		fonts[font][c].Size.x);
	float h = static_cast<float>(
		fonts[font][c].Size.y);

	//tex coords changed, because OpenGL uses a different
	//coordinate system, than the True Type Font
	float vertices[] = {

		//triangle 1:
		//vert upper left, tex lower left
		xpos,     ypos + h,   0.0f, 0.0f,
		//vert lower left, tex upper left
		xpos,     ypos,       0.0f, 1.0f,
		//vert lower right, tex upper right
		xpos + w, ypos,       1.0f, 1.0f,

		//triangle 2:
		//vert lower right, tex upper right
		xpos + w, ypos,       1.0f, 1.0f,
		//vert upper right, tex upper right
		xpos + w, ypos + h,   1.0f, 0.0f,
		//vert upper left, tex lower left
		xpos,     ypos + h,   0.0f, 0.0f
	};

	glGenVertexArrays(1, &fonts[font][c].VAO);
	glGenBuffers(1, &fonts[font][c].VBO);
	glBindVertexArray(fonts[font][c].VAO);
	glBindBuffer(GL_ARRAY_BUFFER, fonts[font][c].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
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

	return fonts[font][c].Advance;
}

int TrueTypeFont::get_descent(const std::string& font)
{
	if (fonts.find(font) == fonts.end())
		return 0; //font not present

	// 'g' is the first character with non zero descent
	Character& c = fonts[font]['g'];
	return c.Size.y - c.Bearing.y;
}

const Shader& TrueTypeFont::get_shader()
{
	return shader;
}
