#pragma once
#include "../stdafx.h"

class CTexture
{
public:
	bool IsLoaded;
	Bitmap* tex;
	std::vector<unsigned char> image;
	unsigned int width, height;

	CTexture(std::string);
	CTexture(std::string, int, int);
	~CTexture(void);
	bool Load(void);
	bool Bind(void);
	GLuint GetId(void);

protected:
	int _magFilter;
	int _minFilter;
	std::string _file;
	GLuint _id;
};

