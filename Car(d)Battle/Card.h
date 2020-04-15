#pragma once
#include "stdafx.h"

class Cards;

class Card
{
	const Cards &cards;
	unsigned int id;
public:
	Card(const Cards&, unsigned int);
	Card(const Card&);//copy constructor

	void draw();
};

