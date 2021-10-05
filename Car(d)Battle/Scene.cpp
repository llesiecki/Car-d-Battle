#include "Scene.h"

Scene::Scene()
{
	background = new Texture("textures\\background.bmp");
	vbo_background = 0;
	vao_background = 0;
	ebo_background = 0;

	shader_bg.load("shaders\\scene_bg_vert.glsl", GL_VERTEX_SHADER);
	shader_bg.load("shaders\\scene_bg_frag.glsl", GL_FRAGMENT_SHADER);
	shader_bg.link();
}

Scene::~Scene()
{
	if (background != nullptr)
		delete background;

	if (vao_background != 0)
		glDeleteVertexArrays(1, &vao_background);

	if (vbo_background != 0)
		glDeleteBuffers(1, &vbo_background);

	if (ebo_background != 0)
		glDeleteBuffers(1, &vbo_background);
}

void Scene::draw(const glm::mat4& proj, const glm::mat4& view)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, background->get_id());
	shader_bg.enable();
	shader_bg.set("TexID", 0);
	shader_bg.set("transform", proj * view);
	glBindVertexArray(vao_background);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, static_cast<void*>(0));
	glDisable(GL_TEXTURE_2D);
}

void Scene::load()
{
	GLfloat vertices_data[] = { //pos.x, pos.y, pos.z, tex.x, tex.y
		-3.2f, -0.005f, -1.8f, 0.0f, 1.0f,
		-3.2f, -0.005f, 1.8f, 0.0f, 0.0f,
		3.2f, -0.005f, 1.8f, 1.0f, 0.0f,
		3.2f, -0.005f, -1.8f, 1.0f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,   // first triangle
		2, 3, 0    // second triangle
	};

	glGenVertexArrays(1, &vao_background);
	glGenBuffers(1, &vbo_background);
	glGenBuffers(1, &ebo_background);

	glBindVertexArray(vao_background);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_background);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data), vertices_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_background);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//attrib 0 - vertex coords
	//attrib num, qty of buffer items, type, normalize?, size of a single vertex, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), static_cast<void*>(0));
	glEnableVertexAttribArray(0);

	//attrib 1 - tex coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	background->load();
	background->bind();
}
