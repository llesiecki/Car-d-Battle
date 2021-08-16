#pragma once

#include "stdafx.h"

class Text
{
	std::string text;
	GLuint VBO, VBO;

public:
	Text();
	Text(const std::string&);
	~Text();

	void draw();
	void set_text(const std::string&);
};
