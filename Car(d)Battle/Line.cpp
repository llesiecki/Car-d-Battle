#include "Line.h"

Line::Line()
    :mvp(), color(), start(), end()
{
    shader.load("shaders/line_vert.glsl", GL_VERTEX_SHADER);
    shader.load("shaders/line_frag.glsl", GL_FRAGMENT_SHADER);
    shader.link();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
}

Line::Line(glm::vec3 start, glm::vec3 end)
{
    this->Line::Line();
    this->start = start;
    this->end = end;
}

Line::Line(glm::vec3 start, glm::vec3 end, glm::vec3 color)
{
    this->Line::Line(start, end);
    this->color = color;
}
