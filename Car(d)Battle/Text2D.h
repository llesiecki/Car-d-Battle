#pragma once

#include "stdafx.h"

struct Text2D
{
	float x, y;
	std::string text;
	void* font;//eg: GLUT_BITMAP_HELVETICA_18, GLUT_BITMAP_TIMES_ROMAN_24

	void render()
	{
		glRasterPos3f(x, y, 1.0f);
		for (char ch : text)
			glutBitmapCharacter(font, ch);
	}
};
