#include "Line.h"

Line::Line()
    :mvp(), color(), start(), end()
{
    shader.load("shaders/line_vert.glsl", GL_VERTEX_SHADER);
    shader.load("shaders/line_frag.glsl", GL_FRAGMENT_SHADER);
    shader.link();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
}

Line::Line(glm::vec3 start, glm::vec3 end)
{
    this->Line::Line();
    this->start = start;
    this->end = end;

    float vertices[] = {
         start.x, start.y, start.z,
         end.x, end.y, end.z,
    };

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Line::Line(glm::vec3 start, glm::vec3 end, glm::vec3 color)
{
    this->Line::Line(start, end);
    this->color = color;
}
