#pragma once

#include "stdafx.h"

struct Text3D
{
	std::string text;
	glm::vec3 pos, rot;
	float angle, scale, line_width;

	inline void render()
	{
		GLfloat width;
		glGetFloatv(GL_LINE_WIDTH, &width);
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(angle, rot.x, rot.y, rot.z);
		glScalef(scale, scale, scale);//text size
		for (char ch : text)
			glutStrokeCharacter(GLUT_STROKE_ROMAN, ch);//GLUT_STROKE_MONO_ROMAN
		glPopMatrix();
		glLineWidth(width);
	}
};

