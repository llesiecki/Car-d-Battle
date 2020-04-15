#include "Card.h"

Card::Card(const Cards& cards, unsigned int id)
    :cards(cards)
{
	this->id = id;
}

Card::Card(const Card& card)
    : cards(card.cards)
{
    id = card.id;
}

void renderStrokeString(float x, float y, const std::string &text)
{
    glPushMatrix();
    glScalef(0.0004f, 0.0004f, 0.0004f);//text size
    for (char ch : text)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, ch);//GLUT_STROKE_MONO_ROMAN
    glPopMatrix();
}

float calc_text_width(std::string str)
{
    float width = 0.0f;
    for (char ch : str)
        width += glutStrokeWidth(GLUT_STROKE_ROMAN, ch)/2500.0f;//(text size)^-1
    return width;
}

void Card::draw()
{
    glPushMatrix();
    glRotatef(90, -1, 0, 0);
    glTranslatef(0.707f / 2, -0.589f, 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cards.back_tex->GetId());
    glCallList(cards.list_back);
    glBindTexture(GL_TEXTURE_2D, cards.cards_texture[id]->GetId());
    glCallList(cards.list_front);
    glBindTexture(GL_TEXTURE_2D, cards.fields_tex->GetId());
    glCallList(cards.list_fields);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0, 0, 0);
    glTranslatef(-0.707f + 0.02f, 0.525f, +0.005f);
    renderStrokeString(0, 0, cards.cards_properties[id][0]);
    glTranslatef(0, -0.589f / 7, 0);//half of height devided by 7 because there is 7 fields
    for (unsigned int i = 1; i < cards.field_names.size(); i++)
    {
        if (!cards.field_names[i].empty())
        {
            renderStrokeString(0, 0, cards.field_names[i] + ":");
            float width = calc_text_width(cards.cards_properties[id][i]);
            glPushMatrix();
            glTranslatef(0.707f - 0.04f - calc_text_width(cards.cards_properties[id][i]), 0, 0);
            renderStrokeString(0, 0, cards.cards_properties[id][i]);
            glPopMatrix();
        }
        glTranslatef(0, -0.589f / 7, 0);//half of height devided by 7 because there is 7 fields
    }
    glPopMatrix();
}
