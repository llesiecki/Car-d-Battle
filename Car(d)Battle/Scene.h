#pragma once
#include "stdafx.h"
#include "utilities/Texture.h"
#include "Shader.h"

class Scene
{
	CTexture* background;
	GLuint vbo_background, vao_background, ebo_background;
	Shader shader_bg;
public:
	Scene();
	~Scene();
	void draw();
	void load();
};

