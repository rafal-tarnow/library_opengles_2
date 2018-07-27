#pragma once

#include <system_opengl_include.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
using namespace std;

typedef struct{
    int numberOfVerticles;
    GLuint vbo_id;
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    GLenum vbo_usage;
    GLenum mode;
    glm::vec4 colour;
} PR_LineStrip;


void PR_init(PR_LineStrip * lineStrip, float * verticlesTable, int tableSize, glm::vec4 color, GLenum mode = GL_LINE_STRIP, GLenum vbo_usage = GL_STATIC_DRAW);
void PR_init(PR_LineStrip * lineStrip, glm::vec3 * verticlesTable, int tableSize, glm::vec4 color, GLenum mode = GL_LINE_STRIP, GLenum vbo_usage = GL_STATIC_DRAW);

void PR_setPointSize(PR_LineStrip * lineStrip, float pointSize);
void PR_setMode(PR_LineStrip * lineStrip, GLenum mode = GL_LINE_STRIP);
void PR_setColour(PR_LineStrip * lineStrip, glm::vec4 colour);
void PR_updateData(PR_LineStrip * lineStrip, glm::vec3 * verticlesTable, int tableSize);
void PR_draw(PR_LineStrip * lineStrip, GLfloat width);
void PR_delete(PR_LineStrip * lineStrip);



