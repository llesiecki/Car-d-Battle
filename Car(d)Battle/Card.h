#pragma once
#include "stdafx.h"
#include "utilities\Vec3.h"

class Cards;

class Card
{
	const Cards &cards;
	unsigned int id;
public:
	Vec3 pos, rot;
	float angle;
	Card(const Cards&, unsigned int&);
	Card(const Card&);//copy constructor
	Card operator=(const Card&);
	void draw();
};