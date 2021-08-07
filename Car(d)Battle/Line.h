#pragma once
#include "stdafx.h"
#include "Shader.h"
#include <array>

class Line
{
	Shader shader;
	GLuint vao, vbo;
    glm::mat4 mvp;
    glm::vec3 color;
    glm::vec3 start;
    glm::vec3 end;

public:
    Line();
    Line(const glm::vec3&, const glm::vec3&);
    Line(const glm::vec3&, const glm::vec3&, const glm::vec3&);
    ~Line();
    void set_MVP(const glm::mat4&);
    void set_color(const glm::vec3&);
    void set_pos(const glm::vec3&, const glm::vec3&);
    void set_start(const glm::vec3&);
    void set_end(const glm::vec3&);
    void draw();

};

