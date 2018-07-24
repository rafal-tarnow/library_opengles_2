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
} LS_LineStrip;


void LS_init(LS_LineStrip * lineStrip, float * verticlesTable, int tableSize, glm::vec4 color, GLenum mode = GL_LINE_STRIP, GLenum vbo_usage = GL_STATIC_DRAW);
void LS_init(LS_LineStrip * lineStrip, glm::vec3 * verticlesTable, int tableSize, glm::vec4 color, GLenum mode = GL_LINE_STRIP, GLenum vbo_usage = GL_STATIC_DRAW);

void LS_setPointSize(LS_LineStrip * lineStrip, float pointSize);
void LS_setMode(LS_LineStrip * lineStrip, GLenum mode = GL_LINE_STRIP);
void LS_setColour(LS_LineStrip * lineStrip, glm::vec4 colour);
void LS_updateData(LS_LineStrip * lineStrip, glm::vec3 * verticlesTable, int tableSize);
void LS_draw(LS_LineStrip * lineStrip, GLfloat width);
void LS_delete(LS_LineStrip * lineStrip);



