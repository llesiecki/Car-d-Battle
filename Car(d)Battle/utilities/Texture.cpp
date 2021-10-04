#include "..\stdafx.h"
#include "Texture.h"
#include "lodepng.h"

CTexture::CTexture(std::string file, int magFilter, int minFilter)
	:bmp(nullptr), png(nullptr)
{
	IsLoaded = false;
	_id = -1;
	_file = file;
	_magFilter = magFilter;
	_minFilter = minFilter;
}

bool CTexture::Load(void)
{
	std::string filename(_file);
	if (filename.substr(filename.size() - 4) != ".png" && filename.substr(filename.size() - 4) != ".PNG")//not a PNG
	{
		if (bmp)
			delete bmp;

		bmp = new Bitmap();
		if (!bmp->loadBMP(_file)) {
			printf("ERROR: Cannot read texture file \"%s\".\n", _file.c_str());
			return false;
		}

		width = bmp->width;
		height = bmp->height;

		IsLoaded = true;

		return true;
	}
	else//PNG texture
	{
		std::vector<unsigned char> lodepng_descriptor;

		if (png)
			delete png;

		png = new std::vector<unsigned char>;
		lodepng::State state;

		unsigned error = lodepng::load_file(lodepng_descriptor, _file); //load the image file with given filename
		if (!error) error = lodepng::decode(*png, width, height, state, lodepng_descriptor);

		if (error)
		{
			std::cerr << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
			return false;
		}

		for (unsigned int i = 0; i < height / 2; i++)
			for (unsigned int j = 0; j < width * 4; j++)
				swap((*png)[i * width * 4 + j], (*png)[(height - i - 1) * width * 4 + j]);

		IsLoaded = true;

		return true;
	}
	return false;
}

void CTexture::Bind(void)
{
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &_id);

	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);

	if (bmp)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp->data);
		delete bmp;
		bmp = nullptr;
	}
	else if (png)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, png->data());
		delete png;
		png = nullptr;
	}

	glGenerateMipmap(GL_TEXTURE_2D);
}


CTexture::~CTexture(void)
{
}


GLuint CTexture::GetId(void)
{
	return _id;
}
