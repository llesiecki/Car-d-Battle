#include "Card.h"

Card::Card(unsigned int id, CTexture* car_tex, CommonCardValues& common_values)
	:common_values(common_values)
{
	this->id = id;
	this->car_tex = car_tex;
	angle = 0.0f;
	highlight = -1;
	invert = true;
}

Card::Card(const Card& card)
	:common_values(card.common_values)
{
	id = card.id;
	highlight = card.highlight;
	car_tex = card.car_tex;
	car_name = card.car_name;
	pos = card.pos;
	rot = card.rot;
	angle = card.angle;
	invert = card.invert;
	values = card.values;
}

Card Card::operator=(const Card& card)
{
	id = card.id;
	highlight = card.highlight;
	car_tex = card.car_tex;
	car_name = card.car_name;
	pos = card.pos;
	rot = card.rot;
	angle = card.angle;
	invert = card.invert;
	values = card.values;
	return Card(*this);
}

void Card::reset_coords()
{
	pos = glm::vec3();
	rot = glm::vec3();
	angle = 0.0f;
}

void Card::highlight_row(int row)
{
	highlight = row;
}

std::string Card::get_category_value(int num)
{
	if (static_cast<unsigned int>(num) >= values.size() || num < 0)
		return std::string();

	return values[num];
}

void Card::draw(const glm::mat4& mvp)
{
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), pos);

	trans *= glm::toMat4(glm::quat(
		glm::vec3(
			-glm::pi<GLfloat>() / 2,
			0.0f,
			0.0f
		)
	));

	if((angle > 0.1 || angle < -0.1) && rot.length() > 0.1)
		trans = glm::rotate(trans, glm::radians(angle), rot);

	if (invert)
		trans = glm::rotate(trans, glm::pi<GLfloat>(),
			glm::vec3(0.0f, 1.0f, 0.0f));

	trans = glm::translate(trans, glm::vec3(
		-CARD_WIDTH / 2,
		-CARD_HEIGHT / 2,
		0.0035f
	));
	
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	common_values.shader->enable();
	common_values.shader->set("TexID", 0);
	common_values.shader->set("transform", mvp * trans);

	glBindVertexArray(common_values.common_vao);
	glBindTexture(GL_TEXTURE_2D, common_values.back_tex->GetId());
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE,
		static_cast<void*>(0));

	glBindTexture(GL_TEXTURE_2D, car_tex->GetId());
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE,
		reinterpret_cast<void*>(1 * 2 * 3 * sizeof(GLubyte)));

	glBindTexture(GL_TEXTURE_2D, common_values.fields_tex->GetId());
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE,
		reinterpret_cast<void*>(2 * 2 * 3 * sizeof(GLubyte)));

	glDisable(GL_TEXTURE_2D);

	float text_scale = 0.001f;
	float margin_horizontal = CARD_WIDTH / 40.0f;
	//48 - font size
	float margin_vertical = ((CARD_HEIGHT / 2 / 7) - 48 * text_scale)/1.5f;

	glm::mat4 fields_trans = glm::translate(trans, glm::vec3(
		//left<>right
		0.0f,
		//set height to the first field
		CARD_HEIGHT / 2.0f * 6.0f / 7.0f,
		//height above the card
		0.001f)
	);

	Text text("", glm::vec4(0, 0, 0, 1));
	text.set_font("arial.ttf");
	text.set_text(car_name);
	text.draw(glm::scale(mvp * glm::translate(fields_trans,
		glm::vec3(
			margin_horizontal,
			margin_vertical,
			0.0f
		)),
		glm::vec3(text_scale))
	);

	for (unsigned int i = 0; i < common_values.field_names.size(); i++)
	{
		//translate to the next field
		//half of height devided by 7 because there are 7 fields
		fields_trans = glm::translate(fields_trans,
			glm::vec3(0.0f,
				-CARD_HEIGHT / 2 / 7,//go one filed downwards
				0.0f
			));

		//highlight the field if selected
		if (highlight == i)
		{
			glLineWidth(1.5f);
			common_values.highlight_line.set_MVP(mvp * fields_trans);
			common_values.highlight_line.draw();
		}

		//draw field name
		text.set_text(common_values.field_names[i] + ":");
		text.draw(glm::scale(mvp * glm::translate(fields_trans,
			glm::vec3(margin_horizontal, margin_vertical, 0.0f)),
			glm::vec3(0.001f)));

		//draw field value
		text.set_text(values[i]);
		float width = static_cast<float>(text.get_width());
		glm::mat4 shift = glm::translate(fields_trans, glm::vec3(
			CARD_WIDTH - margin_horizontal - width * text_scale,
			margin_vertical,
			0.0f
		));
		text.draw(glm::scale(mvp * shift, glm::vec3(0.001f)));
	}
}

void Card::delete_tex()
{
	delete car_tex;
}

void Card::load_tex()
{
	car_tex->Load();
}
