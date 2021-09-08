#include "Line.h"

Line::Line()
	:mvp(), color()
{
	shader.load("shaders/line_vert.glsl", GL_VERTEX_SHADER);
	shader.load("shaders/line_frag.glsl", GL_FRAGMENT_SHADER);

	shader.link();

	indices_num = 0;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
}

Line::Line(const std::vector<glm::vec3>& vertices)
{
	this->Line::Line();
	set_vertices(vertices);
}

Line::Line(const std::vector<glm::vec3>& vertices, const glm::vec3& color)
{
	this->Line::Line(vertices);
	this->color = color;
}

void Line::clean_buffers()
{
	if (vbo != 0)
		glDeleteBuffers(1, &vbo);

	if (ebo != 0)
		glDeleteBuffers(1, &ebo);

	if (vao != 0)
		glDeleteVertexArrays(1, &vao);
}

Line::~Line()
{
	clean_buffers();
}

void Line::set_MVP(const glm::mat4& mvp)
{
	this->mvp = mvp;
}

void Line::set_color(const glm::vec3& color)
{
	this->color = color;
}

void Line::set_vertices(const std::vector<glm::vec3>& vertices, bool cycle)
{
	//generate indices for a series of vertexes
	std::vector<unsigned int> indices;

	indices.push_back(0);//first
	for (unsigned int i = 1; i < vertices.size() - 1; i++)//each next has to be duplicated
	{
		indices.push_back(i);
		indices.push_back(i);
	}
	indices.push_back(vertices.size() - 1);//last

	if (cycle)//closing cycle
	{
		indices.push_back(vertices.size() - 1);//last
		indices.push_back(0);//first
	}

	indices_num = indices.size();
	
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<void *>(0));
	glEnableVertexAttribArray(0);
}

void Line::draw()
{
	shader.enable();
	shader.set("MVP", mvp);
	shader.set("color", color);

	glBindVertexArray(vao);
	glDrawElements(GL_LINES, indices_num, GL_UNSIGNED_INT, static_cast<void*>(0));
}
