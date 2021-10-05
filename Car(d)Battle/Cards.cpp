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
		cards.push_back(Card(row - 1, new Texture("textures\\cars\\" + img_paths[row - 1]), card_values));
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

	card_values.back_tex = new Texture("textures\\" + img_paths.back());
	img_paths.pop_back();
	card_values.fields_tex = new Texture("textures\\" + img_paths.back());
	img_paths.pop_back();

	card_values.vao = card_values.vbo = card_values.ebo = 0;

	card_values.shader = new Shader();
	card_values.shader->load("shaders\\card_vert.glsl", GL_VERTEX_SHADER);
	card_values.shader->load("shaders\\card_frag.glsl", GL_FRAGMENT_SHADER);
	card_values.shader->link();
}

Cards::~Cards()
{
	if (card_values.vao != 0)
		glDeleteVertexArrays(1, &card_values.vao);

	if (card_values.vbo != 0)
		glDeleteBuffers(1, &card_values.vbo);

	if (card_values.ebo != 0)
		glDeleteBuffers(1, &card_values.ebo);


	delete card_values.shader;
	delete card_values.back_tex;
	delete card_values.fields_tex;
	for (Card& card : cards)
		card.delete_tex();
}

bool Cards::load_textures()
{
	bool ret = true;

	std::vector<Texture*> to_load;
	std::list<std::future<bool>> loading;
	to_load.push_back(card_values.back_tex);
	to_load.push_back(card_values.fields_tex);
	for (Card& card : cards)
		to_load.push_back(card.tex());

	unsigned int max_threads = std::thread::hardware_concurrency();
	// leave one thread for others,
	// when more than one thread is available in the system
	if (max_threads > 1)
		--max_threads;
	
	while (!(to_load.empty() && loading.empty()))
	{
		loading.remove_if(
			[&ret](std::future<bool>& fut)
			{
				if (fut.wait_for(std::chrono::milliseconds(0)) ==
					std::future_status::ready)
				{
					ret &= fut.get();
					return true;
				}
				return false;
			}
		);

		while (loading.size() < max_threads && !to_load.empty())
		{
			loading.push_back(std::async(
				std::launch::async,
				&Texture::load,
				to_load.back())
			);
			to_load.pop_back();
		}

		std::this_thread::sleep_for(10ms);
	}

	card_values.back_tex->bind();
	card_values.fields_tex->bind();
	for (Card& card : cards)
		card.tex()->bind();

	return ret;
}

void Cards::create_buffers()
{
	GLfloat vertices_data[] = {	//pos.x, pos.y, tex.x, tex.y
		//rectangle 1:(back)
		0.0f, CARD_HEIGHT,			0.0f, 1.0f,//upper left
		0.0f, 0.0f,					0.0f, 0.0f,//lower left
		CARD_WIDTH, 0.0f,			1.0f, 0.0f,//lower right
		CARD_WIDTH, CARD_HEIGHT,	1.0f, 1.0f,//upper right

		//rectangle 2:(car)
		0.0f, CARD_HEIGHT,				0.0f, 1.0f,//upper left
		0.0f, CARD_HEIGHT / 2,			0.0f, 0.0f,//lower left
		CARD_WIDTH, CARD_HEIGHT / 2,	1.0f, 0.0f,//lower right
		CARD_WIDTH, CARD_HEIGHT,		1.0f, 1.0f,//upper right

		//rectangle 3:(fields)
		0.0f, CARD_HEIGHT / 2,			0.0f, 1.0f,//upper left
		0.0f, 0.0f,						0.0f, 0.0f,//lower left
		CARD_WIDTH, 0.0f,				1.0f, 0.0f,//lower right
		CARD_WIDTH, CARD_HEIGHT / 2,	1.0f, 1.0f,//upper right
	};

	GLubyte indices[] = {
		//indices of first rectangle are reversed, because it's card's back
		2, 1, 0,	// first triangle (drawing first rectangle)
		0, 3, 2,	// second triangle (drawing first rectangle)

		4, 5, 6,	// and so on... (drawing second rectangle)
		6, 7, 4,

		8, 9, 10,
		10, 11, 8
	};

	/// <tech debt>
	/// Consider merging textures into one file 
	/// and change the tex cords respectively.
	/// </tech debt>

	glGenVertexArrays(1, &card_values.vao);
	glGenBuffers(1, &card_values.vbo);
	glGenBuffers(1, &card_values.ebo);

	glBindVertexArray(card_values.vao);

	glBindBuffer(GL_ARRAY_BUFFER, card_values.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data), vertices_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, card_values.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//attrib 0 - vertex coords
	glVertexAttribPointer(
		0, // attrib num
		2, // qty of buffer items
		GL_FLOAT, // type
		GL_FALSE, // normalize?
		4 * sizeof(GLfloat), // stride
		static_cast<void*>(0) // offset
	);
	glEnableVertexAttribArray(0);

	//attrib 1 - tex coords
	glVertexAttribPointer(
		1, // attrib num
		2, // qty of buffer items
		GL_FLOAT, // type
		GL_FALSE, // normalize?
		4 * sizeof(GLfloat), // stride
		reinterpret_cast<void*>(2 * sizeof(float)) // offset
	);
	glEnableVertexAttribArray(1);

	std::vector<glm::vec3> line_corners = {
				{0.0f, 0.0f, 0.0f},
				{CARD_WIDTH, 0.0f, 0.0f},
				{CARD_WIDTH, CARD_HEIGHT / 2 / 7, 0.0f},
				{0.0f, CARD_HEIGHT / 2 / 7, 0.0f},
	};

	card_values.highlight_line.set_color({ 1, 0, 0 });
	card_values.highlight_line.set_vertices(line_corners, true);
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
	if (static_cast<unsigned int>(num) >=
		card_values.field_names.size() || num < 0)
		return std::string();
	return card_values.field_names[num];
}
