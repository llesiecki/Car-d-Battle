#pragma once
#include "stdafx.h"
#include "utilities\Texture.h"
#include "Shader.h"
#include "Line.h"
#include "Text.h"

class Card
{
public:
	struct CommonValues
	{
		std::vector< std::string > field_names;
		CTexture* back_tex, * fields_tex;
		GLuint vbo, vao, ebo;
		Shader* shader;
		Line highlight_line;
	};

private:
	unsigned int id;
	int highlight;
	CTexture* car_tex;
	CommonValues& common_values;
public:
	glm::vec3 pos, rot;
	float angle;
	bool invert;
	std::string car_name;
	std::vector<std::string> values;

	Card() = delete;
	Card(unsigned int, CTexture*, CommonValues&);
	Card(const Card&);
	Card operator=(const Card&);
	void reset_coords();
	void highlight_row(int);
	std::string get_category_value(int num);
	void draw(const glm::mat4&);
	void delete_tex();
	CTexture* tex();
};
