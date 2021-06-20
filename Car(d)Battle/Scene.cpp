#include "Scene.h"

Scene::Scene()
{
	background = new CTexture("textures\\background.bmp");
	vbo_background = -1;

	std::string shader_source =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}";
	shader_bg.load(shader_source, GL_VERTEX_SHADER);

	shader_source =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}";
	shader_bg.load(shader_source, GL_FRAGMENT_SHADER);
	shader_bg.link();
}

Scene::~Scene()
{
	if(vbo_background != -1)
		delete background;
	glDeleteLists(list_background, 1);
}

void Scene::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_background);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<const void *>(0));
	glEnableVertexAttribArray(0);
	shader_bg.enable();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, background->GetId());
		glCallList(list_background);
	glDisable(GL_TEXTURE_2D);
}

void Scene::load()
{
	GLfloat vertices[] = {
		-1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, -1.0f
	};
	glGenBuffers(1, &vbo_background);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_background);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
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
