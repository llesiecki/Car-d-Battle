#include "Text2D.h"

void Text2D::render()
{
	glRasterPos3f(x, y, 0.1f);
	for (char ch : text)
		glutBitmapCharacter(font, ch);
}
