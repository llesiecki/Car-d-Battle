#include "Cards.h"

Cards::Cards(const wchar_t* filename)
{
	WorkBook workbook(filename);
	for (int col = 2; col < workbook.get_col_num(); ++col)
		card_values.field_names.push_back(workbook.cell_to_string(0, col));

	std::vector<std::string> img_paths;

	for (int row = 1; row < workbook.get_row_num(); ++row)
		img_paths.push_back(workbook.cell_to_string(row, 0));

	for (int row = 1; row <= workbook.get_row_num() - 3; ++row)
	{
		cards.push_back(Card(row - 1, new CTexture("textures\\cars\\" + img_paths[row - 1]), card_values));
		cards.back().car_name = workbook.cell_to_string(row, 1);
		for (int col = 2; col < workbook.get_col_num(); ++col)
			cards.back().values.push_back(workbook.cell_to_string(row, col));
	}

	for (unsigned int index = 0; index < card_values.field_names.size(); index++)//add ".0" in acceleration if it's an integer
	{
		if (card_values.field_names[index] == "Acceleration")
		{
			for (auto& card : cards)
			{
				if (card.values[index].find(".") == std::string::npos)
					card.values[index].append(".0");
			}
			break;
		}
	}

	card_values.back_tex = new CTexture("textures\\" + img_paths.back());
	img_paths.pop_back();
	card_values.fields_tex = new CTexture("textures\\" + img_paths.back());
	img_paths.pop_back();

	card_values.common_vao = card_values.common_vbo = card_values.common_ebo = 0;

	std::array<GLfloat, 12> vertices_data = {//pos.x, pos.y, pos.z
		0.0f, 0.0f, 0.0f,
		0.0f, CARD_HEIGHT / 2 / 7, 0.0f,
		CARD_WIDTH, CARD_HEIGHT / 2 / 7, 0.0f,
		CARD_WIDTH, 0.0f, 0.0f
	};

	card_values.frame_vertices = std::move(vertices_data);
}

Cards::~Cards()
{
	if (card_values.common_vao != 0)
		glDeleteVertexArrays(1, &card_values.common_vao);

	if (card_values.common_vbo != 0)
		glDeleteBuffers(1, &card_values.common_vbo);

	if (card_values.common_ebo != 0)
		glDeleteBuffers(1, &card_values.common_ebo);
	delete card_values.back_tex;
	delete card_values.fields_tex;
	for (Card& card : cards)
		card.delete_tex();
}

bool Cards::load_textures()
{
	bool ret = true;
	ret &= card_values.back_tex->Load();
	ret &= card_values.fields_tex->Load();
	for (Card& card : cards)
		card.load_tex();
	
	return ret;
}

void Cards::create_buffers()
{
	GLfloat vertices_data[] = {	//pos.x, pos.y, pos.z, tex.x, tex.y
		0.0f, 0.0f, 0.0f,					1.0f, 0.0f	//rectangle 1:
		-CARD_WIDTH, 0.0f, 0.0f,			0.0f, 0.0f,
		-CARD_WIDTH, CARD_HEIGHT, 0.0f,		0.0f, 1.0f,
		0.0, CARD_HEIGHT, 0.0f,				1.0f, 1.0f,

		0.0f, 0.0f, 0.0f,					1.0f, 0.0f,	//rectangle 2:
		0.0f, CARD_HEIGHT / 2, 0.0f,		1.0f, 1.0f,
		-CARD_WIDTH, CARD_HEIGHT / 2, 0.0f,	0.0f, 1.0f,
		-CARD_WIDTH, 0.0f, 0.0f,			0.0f, 0.0f,

		0.0f, CARD_HEIGHT / 2, 0.0f,		1.0f, 0.0f,	//rectangle 3:
		0.0f, CARD_HEIGHT, 0.0f,			1.0f, 1.0f,
		-CARD_WIDTH, CARD_HEIGHT, 0.0f,		0.0f, 1.0f,
		-CARD_WIDTH, CARD_HEIGHT / 2, 0.0f,	0.0f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2,	// first triangle (drawing first rectangle)
		1, 2, 3,	// second triangle (drawing first rectangle)

		4, 5, 6,	// and so on... (drawing second rectangle)
		5, 6, 7,

		8, 9, 10,
		9, 10, 11,

		12, 13, 14,
		13, 14, 15,
	};

	/// <tech debt>
	/// How to bind multiple textures to one VAO?
	/// If the above is not possible, consider
	/// merging textures into one file and change
	/// the tex cords respectively.
	/// </tech debt>

	glGenVertexArrays(1, &card_values.common_vao);
	glGenBuffers(1, &card_values.common_vbo);
	glGenBuffers(1, &card_values.common_ebo);

	glBindVertexArray(card_values.common_vao);

	glBindBuffer(GL_ARRAY_BUFFER, card_values.common_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data), vertices_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, card_values.common_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//attrib 0 - vertex coords
	//attrib num, qty of buffer items, type, normalize?, size of a single vertex, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), static_cast<void*>(0));
	glEnableVertexAttribArray(0);

	//attrib 1 - tex coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

std::vector<Card> Cards::get_cards_vec()
{
	return cards;
}

void Cards::print()
{
	for (std::string field : card_values.field_names)
	{
		std::cout << field << '\t';
	}
	std::cout << std::endl;
	for (Card& card : cards)
	{
		for (std::string value : card.values)
			std::cout << value << '\t';
	}
	std::cout << '\n';
}

std::string Cards::get_category_name(int num)
{
	if (static_cast<unsigned int>(num) >= card_values.field_names.size() || num < 0)
		return std::string();
	return card_values.field_names[num];
}
