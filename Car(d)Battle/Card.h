#pragma once
#include "stdafx.h"
#include "utilities\Texture.h"
#include "Shader.h"
#include "Line.h"
#include <array>

class  Cards;

struct CommonCardValues
{
	std::vector< std::string > field_names;
	CTexture* back_tex, * fields_tex;
	GLuint common_vbo, common_vao, common_ebo;
	Shader* shader;
	std::string img_path;
	std::array<GLfloat, 4 * 3> frame_vertices; // 4 corners, 3 dimensions each
};

class Card
{
	unsigned int id;
	int highlight;
	CTexture* car_tex;
	CommonCardValues& common_values;
	Line highlight_line;
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
	void draw(const glm::mat4&, const glm::mat4&);
	void delete_tex();
	void load_tex();
};
