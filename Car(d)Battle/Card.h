#pragma once
#include "stdafx.h"
#include "utilities\Texture.h"

class  Cards;

struct CommonCardValues
{
	std::vector< std::string > field_names;
	CTexture* back_tex, * fields_tex;
	GLuint list_front, list_back, list_fields;
	std::string img_path;
};

class Card
{
	unsigned int id;
	int highlight;
	CTexture* car_tex;
	CommonCardValues& common_values;
public:
	glm::vec3 pos, rot;
	float angle;
	bool invert;
	std::string car_name;
	std::vector<std::string> values;

	Card() = delete;
	Card(unsigned int, CTexture*, CommonCardValues&);
	Card(const Card&);//copy constructor
	Card operator=(const Card&);
	void reset_coords();
	void highlight_row(int);
	std::string get_category_value(int num);
	void draw();
	void delete_tex();
	void load_tex();
};
