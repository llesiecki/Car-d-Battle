#include "stdafx.h"
#include "Game.h"


Game Game::instance;
Game& game = Game::get_instance();

void OnRender();
void OnReshape(int, int);

void OnTimerCallback(int id)
{
    OnRender();
	glutTimerFunc(17, OnTimerCallback, 0);
}

POINT cursor, screen;

void glutMouseCallback(int button, int state, int x, int y)
{
    //std::cout << (x*1.0-screen.x/2.0)/screen.y << "\t" << y*1.0f/screen.y << std::endl;
}

void CursorPosUpdateCallback(int x, int y)
{
    cursor = { x, y };
    game.set_cursor_pos(x, y);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(480, 480);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Car(d)Battle");
    game.load();
    game.start(4);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glLineWidth(1.5);
	glutTimerFunc(17, OnTimerCallback, 0);
	glutDisplayFunc(OnRender);
    glutReshapeFunc(OnReshape);
    glutMouseFunc(glutMouseCallback);
    glutPassiveMotionFunc(CursorPosUpdateCallback);
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
        0.0f, 2.3f, 1.15f,//camera position
        0.0f, 0.0f, 0.44f,// camera look-at point
        0.0f, 1.0f, 0.0f //vertical vector
    );

    game.draw();

    glFlush();
    glutSwapBuffers();
}

void OnReshape(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    game.set_screen_size(width, height);
    screen = { width, height };
    gluPerspective(60.0f, static_cast<float>(width) / height, 0.01f, 100.0f);
}