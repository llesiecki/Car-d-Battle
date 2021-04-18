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
	card_values.list_back = card_values.list_fields = card_values.list_front = -1;
}

Cards::~Cards()
{
	if(card_values.list_back != -1)
		glDeleteLists(card_values.list_back, 1);
	if (card_values.list_fields != -1)
		glDeleteLists(card_values.list_fields, 1);
	if (card_values.list_front != -1)
		glDeleteLists(card_values.list_front, 1);
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

void Cards::create_lists()
{
	card_values.list_back = glGenLists(1);
	glNewList(card_values.list_back, GL_COMPILE);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0, 0.0);
				glVertex3f(0.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0, 0.0);
				glVertex3f(-CARD_WIDTH, 0.0f, 0.0f);
			glTexCoord2f(0.0, 1.0);
				glVertex3f(-CARD_WIDTH, CARD_HEIGHT, 0.0f);
			glTexCoord2f(1.0, 1.0);
				glVertex3f(0.0, CARD_HEIGHT, 0.0f);
			glEnd();
	glEndList();

	card_values.list_fields = glGenLists(1);
	glNewList(card_values.list_fields, GL_COMPILE);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0, 0.0);
				glVertex3f(0.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0, 1.0);
				glVertex3f(0.0f, CARD_HEIGHT / 2, 0.0f);
			glTexCoord2f(0.0, 1.0);
				glVertex3f(-CARD_WIDTH, CARD_HEIGHT / 2, 0.0f);
			glTexCoord2f(0.0, 0.0);
				glVertex3f(-CARD_WIDTH, 0.0f, 0.0f);
		glEnd();
	glEndList();

	card_values.list_front = glGenLists(1);
	glNewList(card_values.list_front, GL_COMPILE);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(1.0, 0.0);
				glVertex3f(0.0f, CARD_HEIGHT / 2, 0.0f);
			glTexCoord2f(1.0, 1.0);
				glVertex3f(0.0, CARD_HEIGHT, 0.0f);
			glTexCoord2f(0.0, 1.0);
				glVertex3f(-CARD_WIDTH, CARD_HEIGHT, 0.0f);
			glTexCoord2f(0.0, 0.0);
				glVertex3f(-CARD_WIDTH, CARD_HEIGHT / 2, 0.0f);
		glEnd();
		glPopMatrix();
	glEndList();
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
