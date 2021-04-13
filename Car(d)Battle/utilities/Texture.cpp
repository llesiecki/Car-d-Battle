#include "..\stdafx.h"
#include "Texture.h"
#include "lodepng.h"


CTexture::CTexture(std::string file)
{
	IsLoaded = false;
	tex = nullptr;
	_id = -1;
	_file = file;
	_magFilter = GL_LINEAR;
	_minFilter = GL_LINEAR;
}

CTexture::CTexture(std::string file, int magFilter, int minFilter)
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
		tex = new Bitmap();
		if (!tex->loadBMP(_file)) {
			printf("ERROR: Cannot read texture file \"%s\".\n", _file.c_str());
			return false;
		}
		return true;
	}
	else//PNG texture
	{
		std::vector<unsigned char> png;
		lodepng::State state;
		unsigned error = lodepng::load_file(png, _file); //load the image file with given filename
		if (!error)
			error = lodepng::decode(image, width, height, state, png);
		if (error)
		{
			std::cerr << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
			return false;
		}
		return true;
	}
	return false;
}

bool CTexture::Bind(void)
{
	std::string filename(_file);
	if (filename.substr(filename.size() - 4) != ".png" && filename.substr(filename.size() - 4) != ".PNG")//not a PNG
	{
		glGenTextures(1, &_id);

		glBindTexture(GL_TEXTURE_2D, _id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);

		if (_minFilter == GL_LINEAR_MIPMAP_LINEAR || _minFilter == GL_LINEAR_MIPMAP_NEAREST) {
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex->width, tex->height, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
		}

		delete tex;

		IsLoaded = true;

		return true;
	}
	else//PNG texture
	{
		glGenTextures(1, &_id);

		glBindTexture(GL_TEXTURE_2D, _id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);

		for (unsigned int i = 0; i < height / 2; i++)
			for (unsigned int j = 0; j < width * 4; j++)
				swap(image[i * width * 4 + j], image[(height - i - 1) * width * 4 + j]);
		if (_minFilter == GL_LINEAR_MIPMAP_LINEAR || _minFilter == GL_LINEAR_MIPMAP_NEAREST) {
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
		}

		IsLoaded = true;
		image.clear();

		return true;
	}
	return false;
}


CTexture::~CTexture(void)
{
}


GLuint CTexture::GetId(void)
{
	return _id;
}
