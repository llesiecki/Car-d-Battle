#include "stdafx.h"
#include "Texture.h"
void OnRender();

void OnTimerCallback(int id)
{
    OnRender();
	glutTimerFunc(17, OnTimerCallback, 0);
}

GLuint _displayListId; // Id displaylisty.
CTexture* _wallTexture; // Tekstura.

int main(int argc, char* argv[])
{
	Cards card;
	WorkBook workbook(L"arkusz.xls");
	workbook.to_Cards(card);
	card.print();
    
	glutInit(&argc, argv);
	glutInitWindowSize(480, 480);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Car(d)Battle");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    char filename[] = "lada.bmp";
    _wallTexture = new CTexture(filename);
    _wallTexture->Load();
    _displayListId = glGenLists(1); // Id displaylisty.

    glNewList(_displayListId, GL_COMPILE);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
                glVertex3f(-0.5f, -0.5f, 0.0f);
            glTexCoord2f(0.0, 1.0);
                glVertex3f(0.5f, -0.5f, 0.0f);
            glTexCoord2f(1.0, 1.0);
                glVertex3f(0.5f, 0.5f, 0.0f);
            glTexCoord2f(1.0, 0.0);
                glVertex3f(-0.5f, 0.5f, 0.0f);
        glEnd();
    glEndList();

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
    glBindTexture(GL_TEXTURE_2D, _wallTexture->GetId());
    glPushMatrix();
    glRotatef(GetTickCount()/10.0f, 1,1,1);
    glCallList(_displayListId);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}