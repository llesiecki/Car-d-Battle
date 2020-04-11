#pragma once

#include "stdafx.h"
#include "Texture.h"

class Cards
{
public:
	std::vector< std::string > field_names, img_paths;
	std::vector< std::vector<std::string> > cards_properties;
	CTexture *back_tex, *fields_tex;
	std::vector<CTexture*> cards_texture;
	GLuint list_front, list_back, list_fields;
public:
	Cards(const wchar_t* filename);
	bool load_textures();
	void create_lists();
	void print();
	friend class WorkBook;
};
