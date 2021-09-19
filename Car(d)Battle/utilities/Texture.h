#pragma once

class CTexture
{
public:
	bool IsLoaded;

	CTexture(std::string);
	CTexture(std::string, int, int);
	~CTexture(void);
	bool Load(void);
	GLuint GetId(void);

protected:
	int _magFilter;
	int _minFilter;
	std::string _file;
	GLuint _id;
};

