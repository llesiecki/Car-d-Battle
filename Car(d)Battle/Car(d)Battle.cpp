#include "stdafx.h"
#include "Game.h"

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 720);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Car(d)Battle");
    static Game game;//See Note 1
    game.load();
    game.start(4);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glLineWidth(1.5);

    static void (*render_timer)(int) = [](int plce_holder){//See Note 1
        game.draw();
        glutTimerFunc(17, render_timer, 0); };

	glutTimerFunc(17, render_timer, 0);
	glutDisplayFunc([]() {return game.draw(); });
    glutReshapeFunc([](int w, int h) {return game.set_screen_size(w, h); });
    glutPassiveMotionFunc([](int x, int y) {return game.set_cursor_pos(x, y); });
	glutMainLoop();
	return 0;
}

/// Note 1:
/// Must be static as it must be used in lambdas without capture lists.
/// Capture lists cannot be used in these lambdas, because they are passed as C-style function pointers.
