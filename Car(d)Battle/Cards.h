#pragma once

#include "stdafx.h"
#include "utilities\Texture.h"
#include "WorkBook.h"
#include "Card.h"

class Cards
{
	std::vector<Card> cards;
	Card::CommonValues card_values;

public:
	Cards(const wchar_t* filename);
	~Cards();
	bool load_textures();
	void create_buffers();
	std::vector<Card> get_cards_vec();
	void print();
	std::string get_category_name(int num);
};
