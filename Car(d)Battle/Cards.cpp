#include "stdafx.h"
#include "Cards.h"

Cards::Cards(const wchar_t* filename)
{
	WorkBook workbook(L"arkusz.xls");
	workbook.to_Cards(*this);
	back_tex = new CTexture(img_paths.back());
	img_paths.pop_back();
	fields_tex = new CTexture(img_paths.back());
	img_paths.pop_back();
	for (std::string img_path : img_paths)
		cards_texture.push_back(new CTexture(img_path));
}

bool Cards::load_textures()
{
	bool ret = true;
	ret &= back_tex->Load();
	ret &= fields_tex->Load();
	/*for (auto tex : cards_texture)
		ret &= tex->Load();
	*/
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
				glVertex3f(-0.707f, 0.0f, 0.0f);
			glTexCoord2f(0.0, 1.0);
				glVertex3f(-0.707f, 1.178f, 0.0f);
			glTexCoord2f(1.0, 1.0);
				glVertex3f(0.0, 1.178f, 0.0f);
			glEnd();
	glEndList();

	list_fields = glGenLists(1);
	glNewList(list_fields, GL_COMPILE);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0, 0.0);
				glVertex3f(0.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0, 1.0);
				glVertex3f(0.0, 0.589, 0.0f);
			glTexCoord2f(0.0, 1.0);
				glVertex3f(-0.707f, 0.589, 0.0f);
			glTexCoord2f(0.0, 0.0);
				glVertex3f(-0.707f, 0.0f, 0.0f);
		glEnd();
	glEndList();

	list_front = glGenLists(1);
	glNewList(list_front, GL_COMPILE);
	glPushMatrix();
	//glTranslatef(0, -1.0f, 0);
		glBegin(GL_QUADS);

			glTexCoord2f(1.0, 0.0);
				glVertex3f(0.0f, 0.589, 0.0f);
			glTexCoord2f(1.0, 1.0);
				glVertex3f(0.0, 1.178f, 0.0f);
			glTexCoord2f(0.0, 1.0);
				glVertex3f(-0.707f, 1.178f, 0.0f);
			glTexCoord2f(0.0, 0.0);
				glVertex3f(-0.707f, 0.589, 0.0f);
		glEnd();
		glPopMatrix();
	glEndList();
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