#pragma once
#include "..\stdafx.h"
#include "Bitmap.h"
#include "lodepng.h"

class Texture
{
	size_t width, height;
	GLuint id;
	Bitmap* bmp;
	std::vector<GLubyte>* png;
	std::string path;

	inline bool ends_with(std::string const&, std::string const&);

public:
	Texture(std::string);
	~Texture();
	bool load();
	void bind(int = GL_LINEAR, int = GL_LINEAR);
	GLuint get_id();
	bool is_loaded();
};
