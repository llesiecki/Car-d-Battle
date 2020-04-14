#pragma once
#include "stdafx.h"
#include "utilities/Texture.h"

class Scene
{
	CTexture* background;
	GLuint list_background;
public:
	Scene();
	~Scene();
	void draw();
	void load();
};

