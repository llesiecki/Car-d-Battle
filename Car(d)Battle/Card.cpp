#include "Card.h"

Card::Card(Cards *cards, unsigned int id)
{
    this->cards = cards;
	this->id = id;
}

void renderBitmapString(float x, float y, std::string text)
{
    glRasterPos3f(x, y, 0.0f);
    glPushMatrix();
    glScalef(1 / 152.38f, 1 / 152.38f, 1 / 152.38f);
    glScalef(0.06f, 0.06f, 0.06f);
    for (char ch : text)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, ch);//GLUT_STROKE_MONO_ROMAN
    glPopMatrix();
}

float calc_text_width(std::string str)
{
    float width = 0.0f;
    for (char ch : str)
        width += glutStrokeWidth(GLUT_STROKE_ROMAN, ch)/2540.0f;
    return width;
}

void Card::draw()
{
    glPushMatrix();
    glRotatef(90, -1, 0, 0);
    glTranslatef(0.707f / 2, -0.589, 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cards->back_tex->GetId());
    glCallList(cards->list_back);
    glBindTexture(GL_TEXTURE_2D, cards->cards_texture[id]->GetId());
    glCallList(cards->list_front);
    glBindTexture(GL_TEXTURE_2D, cards->fields_tex->GetId());
    glCallList(cards->list_fields);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0, 0, 0);
    glTranslatef(-0.707f + 0.02f, 0.525f, +0.005f);
    renderBitmapString(0, 0, cards->cards_properties[id][0]);
    glTranslatef(0, -0.589f / 7, 0);
    for (unsigned int i = 1; i < cards->field_names.size(); i++)
    {
        renderBitmapString(0, 0, cards->field_names[i] + ":");
        float width = calc_text_width(cards->cards_properties[id][i]);
        glPushMatrix();
        glTranslatef(0.707f - 0.04f - calc_text_width(cards->cards_properties[id][i]), 0, 0);
        renderBitmapString(0, 0, cards->cards_properties[id][i]);
        glPopMatrix();
        glTranslatef(0, -0.589f / 7, 0);
    }
    glPopMatrix();
}
