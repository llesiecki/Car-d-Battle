#pragma once
#include "utilities/Texture.h"
#include "utilities/Vec3.h"

class IRenderableObiect
{
protected:
	CTexture* tex;
	GLuint disp_list;
	Vec3 pos, rot;
	bool is_loaded;
	virtual void render() = 0;
	virtual void load() = 0;
};
