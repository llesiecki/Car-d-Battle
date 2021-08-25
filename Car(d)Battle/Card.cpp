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
    common_values = card.common_values;
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

void Card::draw(const glm::mat4& proj, const glm::mat4& view)
{
    glm::mat4 trans(1.0f);
    trans = glm::translate(trans, pos);
    trans = glm::rotate(trans, glm::radians(angle), rot);
    if (invert)
        trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, common_values.back_tex->GetId());
    common_values.shader->enable();
    common_values.shader->set("TexID", 0);
    common_values.shader->set("transform", proj * view * trans);
    glBindVertexArray(common_values.common_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, static_cast<void*>(0));
    glDisable(GL_TEXTURE_2D);

    trans = glm::translate(trans, glm::vec3(-CARD_WIDTH + 0.02f, 0.525f, 0.0035f));
    renderStrokeString(0, 0, car_name);
    trans = glm::translate(trans, glm::vec3(0, -CARD_HEIGHT / 2 / 7, 0));//half of height devided by 7 because there are 7 fields
    for (unsigned int i = 0; i < common_values.field_names.size(); i++)
    {
        if (highlight == i)
        {

            glLineWidth(1.5f);
            glm::vec3 line_corners[] = {
                {0.0f, 0.0f, 0.0f},
                {0.0f, CARD_HEIGHT / 2 / 7, 0.0f},
                {CARD_WIDTH, CARD_HEIGHT / 2 / 7, 0.0f},
                {CARD_WIDTH, 0.0f, 0.0f}
            };

            for (glm::vec3& corner : line_corners)//tanslate the highlight
                corner += glm::vec3(-0.02f, -0.02f, 0.0035f);

            highlight_line.set_color({ 1, 0, 0 });
            highlight_line.set_MVP(trans);
            highlight_line.set_pos(line_corners[0], line_corners[1]);
            highlight_line.draw();
            highlight_line.set_pos(line_corners[1], line_corners[2]);
            highlight_line.draw();
            highlight_line.set_pos(line_corners[2], line_corners[3]);
            highlight_line.draw();
            highlight_line.set_pos(line_corners[3], line_corners[0]);
            highlight_line.draw();
        }
        renderStrokeString(0, 0, common_values.field_names[i] + ":");
        float width = calc_text_width(values[i]);
        glPushMatrix();
        glTranslatef(CARD_WIDTH - 0.04f - width, 0, 0);
        renderStrokeString(0, 0, values[i]);
        glPopMatrix();
        glTranslatef(0, -CARD_HEIGHT / 2 / 7, 0);//half of height devided by 7 because there are 7 fields
    }
    glPopMatrix();
}

void Card::delete_tex()
{
    delete car_tex;
}

void Card::load_tex()
{
    car_tex->Load();
}
