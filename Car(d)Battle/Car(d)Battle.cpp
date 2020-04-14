#include "stdafx.h"
#include "Scene.h"
#include <time.h>

void OnRender();
void OnReshape(int, int);

void OnTimerCallback(int id)
{
    OnRender();
	glutTimerFunc(17, OnTimerCallback, 0);
}

std::vector<Card> card_vec;
std::vector<std::pair<float, float>> random_translation_vec;
Scene scene;

int main(int argc, char* argv[])
{
    srand(time(NULL));

    for (int i = 0; i < 24; i++)
        random_translation_vec.push_back(std::pair<float, float>(rand()/1000000.0f - RAND_MAX / 2000000.0f, rand() / 1000000.0f - RAND_MAX / 2000000.0f));
    Cards cards(L"carlist.xls");
	cards.print();
	glutInit(&argc, argv);
	glutInitWindowSize(480, 480);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Car(d)Battle");
    cards.load_textures();
    cards.create_lists();
    scene.load();
    card_vec = cards.get_cards_vec();
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glLineWidth(1.5);
	glutTimerFunc(17, OnTimerCallback, 0);
	glutDisplayFunc(OnRender);
    glutReshapeFunc(OnReshape);
	glutMainLoop();
	return 0;
}

void draw_player_stacks(int num)
{
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.0f);
    glTranslatef(1.0f, 0.0f, 0.5f);
    for (int i = 0; i < 10; i++)
    {
        glPushMatrix();
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        card_vec[(GetTickCount() / 10 / 360 + i) % card_vec.size()].draw();
        glPopMatrix();
        glTranslatef(random_translation_vec[i].first, 0.005f, random_translation_vec[i].second);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.7f);
    glTranslatef(-1.0f, 0.0f, -0.5f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    for (int i = 0; i < 10; i++)
    {
        glPushMatrix();
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        card_vec[(GetTickCount() / 10 / 360 + i) % card_vec.size()].draw();
        glPopMatrix();
        glTranslatef(random_translation_vec[i].first, 0.005f, random_translation_vec[i].second);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.4f, 0.0f, 0.3f);
    glTranslatef(-0.5f, 0.0f, 1.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    for (int i = 0; i < 10; i++)
    {
        glPushMatrix();
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        card_vec[(GetTickCount() / 10 / 360 + i) % card_vec.size()].draw();
        glPopMatrix();
        glTranslatef(random_translation_vec[i].first, 0.005f, random_translation_vec[i].second);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.4f, 0.0f, 0.3f);
    glTranslatef(0.5f, 0.0f, -1.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    for (int i = 0; i < 10; i++)
    {
        glPushMatrix();
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        card_vec[(GetTickCount() / 10 / 360 + i) % card_vec.size()].draw();
        glPopMatrix();
        glTranslatef(random_translation_vec[i].first, 0.005f, random_translation_vec[i].second);
    }
    glPopMatrix();
}

void draw_center_stack(int num)
{
    glPushMatrix();
    for (int i = 0; i < 24; i++)
    {
        glPushMatrix();
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        card_vec[(GetTickCount() / 10 / 360 + i) % card_vec.size()].draw();
        glPopMatrix();
        glTranslatef(random_translation_vec[i].first, 0.001f, random_translation_vec[i].second);
    }
    glPopMatrix();
}

void OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0f, 2.3f, 1.15f,//camera position
        0.0f, 0.0f, 0.44f,// camera look-at point
        0.0f, 1.0f, 0.0f //vertical vector
    );
    scene.draw();

    draw_player_stacks(4);
    draw_center_stack(1);

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.0f);
    card_vec[(GetTickCount() / 10 / 360) % card_vec.size()].draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.7f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    card_vec[(GetTickCount() / 10 / 360) % card_vec.size()].draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.4f, 0.0f, 0.3f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    card_vec[(GetTickCount() / 10 / 360) % card_vec.size()].draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.4f, 0.0f, 0.3f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    card_vec[(GetTickCount() / 10 / 360) % card_vec.size()].draw();
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void OnReshape(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(60.0f, static_cast<float>(width) / height, 0.01f, 100.0f);
}