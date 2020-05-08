#include "Scene.h"

Scene::Scene()
{
	background = new CTexture("textures\\background.bmp");
	list_background = -1;
}

Scene::~Scene()
{
	if(list_background != -1)
		delete background;
	glDeleteLists(list_background, 1);
}

void Scene::draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, background->GetId());
		glCallList(list_background);
	glDisable(GL_TEXTURE_2D);
}

void Scene::load()
{
	list_background = glGenLists(1);
	glNewList(list_background, GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.0f, -0.005f, 0.0f);
	glScalef(16.0f, 0.0f, 9.0f);//texture format
	glScalef(0.2f, 0.0f, 0.2f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
				glVertex3f(-1.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0, 0.0);
				glVertex3f(-1.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0, 0.0);
				glVertex3f(1.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0, 1.0);
				glVertex3f(1.0f, 0.0f, -1.0f);
		glEnd();
	glPopMatrix();
	glEndList();
	background->Load();
}
