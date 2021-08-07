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

Line::Line(const glm::vec3& start, const glm::vec3& end)
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

Line::Line(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color)
{
    this->Line::Line(start, end);
    this->color = color;
}

Line::~Line()
{
    if (vao != 0)
        glDeleteVertexArrays(1, &vao);

    if (vbo != 0)
        glDeleteVertexArrays(1, &vbo);
}

void Line::set_MVP(const glm::mat4& mvp)
{
    this->mvp = mvp;
}

void Line::set_color(const glm::vec3& color)
{
    this->color = color;
}

void Line::set_pos(const glm::vec3& start, const glm::vec3& end)
{
    this->start = start;
    this->end = end;
}

void Line::set_start(const glm::vec3& start)
{
    this->start = start;
}

void Line::set_end(const glm::vec3& end)
{
    this->end = end;
}

void Line::draw()
{
    shader.enable();
    shader.set("MVP", mvp);
    shader.set("color", color);

    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);
    shader.disable();
}
