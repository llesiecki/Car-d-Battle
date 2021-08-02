#pragma once
#include "stdafx.h"
#include "Shader.h"
#include <array>

class Line
{
	Shader shader;
	GLuint vao, vbo;
    std::vector<float> vertices;
    glm::mat4 mvp;
    glm::vec3 color;

public:
    Line();
    Line(glm::vec3, glm::vec3);
    ~Line();
    void set_MVP(glm::mat4);
    void set_color(glm::vec3);
    void set_pos(glm::vec3, glm::vec3);
    void draw();

};

