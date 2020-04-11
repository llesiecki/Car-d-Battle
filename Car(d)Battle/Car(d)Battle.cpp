#include "stdafx.h"
#include "Texture.h"
void OnRender();

void OnTimerCallback(int id)
{
    OnRender();
	glutTimerFunc(17, OnTimerCallback, 0);
}

Cards cards(L"arkusz.xls");
int main(int argc, char* argv[])
{
	cards.print();
	glutInit(&argc, argv);
	glutInitWindowSize(480, 480);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Car(d)Battle");
    cards.load_textures();
    cards.create_lists();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    

	glutTimerFunc(17, OnTimerCallback, 0);
	glutDisplayFunc(OnRender);
	glutMainLoop();
	return 0;
}

void OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glPushMatrix();
    glTranslatef(0, -0.7, 0);
    glRotatef(GetTickCount() / 10.0f, 0, 1, 0);
        glBindTexture(GL_TEXTURE_2D, cards.back_tex->GetId());
        glCallList(cards.list_front);
        glCallList(cards.list_back);
        glBindTexture(GL_TEXTURE_2D, cards.fields_tex->GetId());
        glCallList(cards.list_fields);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    glFlush();
    glutSwapBuffers();
}