#include "TrueTypeFont.h"

TrueTypeFont::TrueTypeFont()
{
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        throw std::string("ERROR::FREETYPE: Could not init FreeType Library");
    }
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
            face->glyph->advance.x
        };

        fonts[name][c] = character;
    }

    FT_Done_Face(face);
}