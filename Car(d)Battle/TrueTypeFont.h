#pragma once

#include "stdafx.h"
#include "Singleton.h"
#include "Shader.h"
#include <map>
#include <array>
#include <ft2build.h>
#include FT_FREETYPE_H

class TrueTypeFont
{
	struct Character {
		GLuint                  TextureID;  // ID handle of the glyph texture
		glm::ivec2              Size;       // Size of glyph
		glm::ivec2				Bearing;    // Offset from baseline to left/top of glyph
		FT_Pos                  Advance;    // Offset to advance to next glyph
		GLuint                  VBO;
		GLuint                  VAO;
	};

	FT_Library ft;
	std::map<std::string, std::array<Character, 128>> fonts;
	Shader shader;

	TrueTypeFont();
	~TrueTypeFont();

	friend TrueTypeFont& Singleton<TrueTypeFont>();

public:
	void load_font(const std::string&, const std::string&);
	GLuint get_VAO(const std::string&, char);
	GLuint get_tex_id(const std::string&, char);
	int get_char_width(const std::string&, char);
	const Shader& get_shader();
};
