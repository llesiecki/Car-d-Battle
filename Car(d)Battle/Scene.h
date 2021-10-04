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
	void draw(const glm::mat4&, const glm::mat4&);
	void load();
};

