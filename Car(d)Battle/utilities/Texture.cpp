#include "Texture.h"

Texture::Texture(std::string path)
	:bmp(nullptr), png(nullptr), id(0), path(path), width(0), height(0)
{
}

Texture::~Texture()
{
	if (bmp)
		delete bmp;

	if (png)
		delete png;

	if (id)
		glDeleteTextures(1, &id);
}

inline bool Texture::ends_with(
	std::string const& value,
	std::string const& ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool Texture::load()
{
	if (ends_with(path, ".bmp") || ends_with(path, ".BMP"))
	{
		if (bmp)
			delete bmp;

		bmp = new Bitmap();
		if (!bmp->loadBMP(path)) {
			std::cerr << "ERROR: Cannot read texture file: "
				<< path << std::endl;
			return false;
		}

		width = bmp->width;
		height = bmp->height;

		return true;
	}
	
	if (ends_with(path, ".png") || ends_with(path, ".PNG"))
	{
		std::vector<unsigned char> png_coded;

		if (png)
			delete png;

		png = new std::vector<GLubyte>;
		lodepng::State state;

		// load the image file with given filename
		unsigned int error = lodepng::load_file(png_coded, path);
		if (!error)
			error = lodepng::decode(*png, width, height, state, png_coded);

		if (error)
		{
			std::cerr << "Decoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;
			return false;
		}

		// Flip the texture vertically, because lodepng decodes it upside down
		//
		// Old code, it took 210ms to flip a 707 x 589 image @ i7-4770K
		// for (size_t i = 0; i < height / 2; i++)
		//     for (size_t j = 0; j < width * 4; j++)
		//         std::swap(
		//             (*png)[i * width * 4 + j],
		//             (*png)[(height - i - 1) * width * 4 + j]);
		//
		// New code, it takes 3.6ms to flip a 707 x 589 image @ i7-4770K
		GLubyte* img = png->data();
		size_t offset = width * 4 * (height - 1);
		for (size_t row = 0; row < width * 4 * height / 2; row += width * 4)
		{
			GLubyte* upper = &img[row];
			GLubyte* lower = &img[offset - row];
			for (size_t col = 0; col < width * 4; col++)
			{
				GLubyte tmp = *upper;
				*upper = *lower;
				*lower = tmp;
				lower++;
				upper++;
			}
		}
		return true;
	}
	return false;
}

void Texture::bind(int minFilter, int magFilter)
{
	glEnable(GL_TEXTURE_2D);

	if (id)
		glDeleteTextures(1, &id);

	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);

	if (bmp)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, bmp->data);
		delete bmp;
		bmp = nullptr;
	}
	else if (png)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, png->data());
		delete png;
		png = nullptr;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glGenerateMipmap(GL_TEXTURE_2D);
}

GLuint Texture::get_id()
{
	return id;
}

bool Texture::is_loaded()
{
	return bmp || png;
}
