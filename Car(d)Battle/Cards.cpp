#include "stdafx.h"
#include "Cards.h"
#include <future>
#include <list>

Cards::Cards(const wchar_t* filename)
{
	WorkBook workbook(filename);
	workbook.to_Cards(*this);
	back_tex = new CTexture("textures\\" + img_paths.back());
	img_paths.pop_back();
	fields_tex = new CTexture("textures\\" + img_paths.back());
	img_paths.pop_back();
	for (std::string img_path : img_paths)
		cards_texture.push_back(new CTexture("textures\\cars\\" + img_path));
	list_back = list_fields = list_front = -1;
}

Cards::~Cards()
{
	if(list_back != -1)
		glDeleteLists(list_back, 1);
	if (list_fields != -1)
		glDeleteLists(list_fields, 1);
	if (list_front != -1)
		glDeleteLists(list_front, 1);
	delete back_tex;
	delete fields_tex;
	for (CTexture*& tex : cards_texture)
		delete tex;
}

bool Cards::load_textures()
{
	bool ret = true;
	std::vector<CTexture*> to_load;
	std::list<std::future<bool>> loading;
	to_load = cards_texture;
	to_load.push_back(back_tex);
	to_load.push_back(fields_tex);

	unsigned int max_threads = std::thread::hardware_concurrency();
	if (max_threads > 1)//leave one thread for others, when more than one thread is available in the system
		--max_threads;

	for (auto it = to_load.begin(); it != to_load.end(); ++it)
	{
		while (loading.size() >= max_threads)
		{
			std::this_thread::sleep_for(100ms);
			loading.remove_if(
				[](const std::future<bool>& fut)
				{ return fut.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready; });
		}
		loading.push_back(std::async(std::launch::async, &CTexture::Load, *it));
	}
	while (!loading.empty())
	{
		std::this_thread::sleep_for(100ms);
		loading.remove_if(
			[](const std::future<bool>& fut)
			{ return fut.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready; });
	}
	for (auto tex : cards_texture)
		tex->Bind();
	back_tex->Bind();
	fields_tex->Bind();

	return ret;
}

void Cards::create_lists()
{
	list_back = glGenLists(1);
	glNewList(list_back, GL_COMPILE);
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

	list_fields = glGenLists(1);
	glNewList(list_fields, GL_COMPILE);
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

	list_front = glGenLists(1);
	glNewList(list_front, GL_COMPILE);
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
	std::vector<Card> cards;
	for (unsigned int i = 0; i < cards_texture.size(); i++)
		cards.push_back(Card(*this, i));
	return cards;
}

void Cards::print()
{
	for (std::string field : field_names)
	{
		std::cout << field << '\t';
	}
	std::cout << std::endl;
	for (unsigned int i = 0; i < cards_properties.size(); i++)
	{
		//std::cout << "path : " << img_paths[i] << '\t';
		for (unsigned int j = 0; j < cards_properties[i].size(); j++)
			std::cout << cards_properties[i][j] << '\t';
	}
	std::cout << '\n';
}

std::string Cards::get_category_name(int num)
{
	num++;
	if (static_cast<unsigned int>(num) >= field_names.size() || num < 0)
		return std::string();
	return field_names[num];
}

