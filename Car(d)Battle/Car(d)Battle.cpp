#include "stdafx.h"

void OnRender();

void OnTimerCallback(int id)
{
	glutTimerFunc(17, OnTimerCallback, 0);
}

int main(int argc, char* argv[])
{
	Cards card;
	WorkBook workbook(L"arkusz.xls");
	workbook.to_Cards(card);
	card.print();
	glutInit(&argc, argv);
	glutInitWindowSize(480, 480);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Car(d)Battle");
	glutTimerFunc(17, OnTimerCallback, 0);
	glutDisplayFunc(OnRender);
	glutMainLoop();
	return 0;
}

void OnRender() {
	glClear(GL_COLOR_BUFFER_BIT);

	glutSolidCube(0.2f);

	glFlush();
	glutSwapBuffers();
}
