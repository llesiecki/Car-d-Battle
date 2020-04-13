#include "stdafx.h"
#include "utilities\Texture.h"
void OnRender();
void OnReshape(int, int);

void OnTimerCallback(int id)
{
    OnRender();
	glutTimerFunc(17, OnTimerCallback, 0);
}

std::vector<Card> card_vec;

int main(int argc, char* argv[])
{
    Cards cards(L"carlist.xls");
	cards.print();
	glutInit(&argc, argv);
	glutInitWindowSize(480, 480);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Car(d)Battle");
    cards.load_textures();
    cards.create_lists();
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

void OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0f, 5.0f, 15.0f,//camera position
        0.0f, 0.0f, 0.0f,// camera look-at point
        0.0f, 1.0f, 0.0f //vertical vector
    );
    glPushMatrix();
        glTranslatef(0, -0.7f, 0);
        glRotatef(GetTickCount() / 10.0f+180, 0, 1, 0);
        glTranslatef(0.707f/2, 0, 0);
        card_vec[(GetTickCount() / 10/360) % card_vec.size()].draw();
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