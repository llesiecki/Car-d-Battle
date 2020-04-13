#pragma once
#include "stdafx.h"

class Cards;

class Card
{
	Cards *cards;
	unsigned int id;
public:
	Card(Cards*, unsigned int);

	void draw();
};

