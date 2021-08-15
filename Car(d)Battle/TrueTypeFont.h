#pragma once

#define GL_CLAMP_TO_EDGE 0x812F 

#include "stdafx.h"
#include "Singleton.h"
#include "Shader.h"
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

class TrueTypeFont
{
    struct Character {
        GLuint                  TextureID;  // ID handle of the glyph texture
        std::pair<int, int>     Size;       // Size of glyph
        std::pair<int, int>     Bearing;    // Offset from baseline to left/top of glyph
        FT_Pos                  Advance;    // Offset to advance to next glyph
    };

    FT_Library ft;
    std::map<std::string, std::map<char, Character>> fonts;
    Shader shader;

    TrueTypeFont();
    ~TrueTypeFont();

    friend TrueTypeFont& Singleton<TrueTypeFont>();

public:
    void load_font(const std::string&, const std::string&);
    void draw(const std::string&, const std::string&, const glm::vec3&);
};
