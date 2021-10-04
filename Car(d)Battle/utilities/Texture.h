#pragma once

class CTexture
{
public:
	bool IsLoaded;

	CTexture(std::string, int = GL_LINEAR, int = GL_LINEAR);
	~CTexture(void);
	bool Load(void);
	void Bind(void);
	GLuint GetId(void);

protected:
	int _magFilter;
	int _minFilter;
	size_t width, height;
	std::string _file;
	GLuint _id;
	Bitmap* bmp;
	std::vector<unsigned char>* png;
};
