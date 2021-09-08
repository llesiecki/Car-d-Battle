#pragma once
#include "stdafx.h"
#include "Shader.h"
#include <array>

class Line
{
	Shader shader;
	GLuint vao, vbo, ebo;
    glm::mat4 mvp;
    glm::vec3 color;
    unsigned int indices_num;

    void clean_buffers();

public:
    Line();
    Line(const std::vector<glm::vec3>&);
    Line(const std::vector<glm::vec3>&, const glm::vec3&);
    ~Line();
    void set_MVP(const glm::mat4&);
    void set_color(const glm::vec3&);
    void set_vertices(const std::vector<glm::vec3>&, bool = false);
    void draw();
};

