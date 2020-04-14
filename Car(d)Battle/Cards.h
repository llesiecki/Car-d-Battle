#pragma once

#include "stdafx.h"
#include "utilities\Texture.h"

class Card;

class Cards
{
	std::vector< std::string > field_names, img_paths;
	std::vector< std::vector<std::string> > cards_properties;
	CTexture *back_tex, *fields_tex;
	std::vector<CTexture*> cards_texture;
	GLuint list_front, list_back, list_fields;
public:
	Cards(const wchar_t* filename);
	~Cards();
	bool load_textures();
	void create_lists();
	std::vector<Card> get_cards_vec();
	void print();
	friend class WorkBook;
	friend class Card;
};
