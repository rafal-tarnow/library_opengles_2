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
    GLuint texture_id = 0;
} Primitive;


void PR_init(Primitive * primitive, float * verticlesTable, int tableSize, glm::vec4 color, GLenum mode = GL_LINE_STRIP, GLenum vbo_usage = GL_STATIC_DRAW);
void PR_init(Primitive * primitive, glm::vec3 * verticlesTable, int tableSize, glm::vec4 color, GLenum mode = GL_LINE_STRIP, GLenum vbo_usage = GL_STATIC_DRAW);

void PR_setPointSize(Primitive * primitive, float pointSize);
void PR_setMode(Primitive * primitive, GLenum mode = GL_LINE_STRIP);
void PR_setColour(Primitive * primitive, glm::vec4 colour);
void PR_setVerticles(Primitive * primitive, glm::vec3 * verticlesTable, int tableSize);
void PR_setTexture(Primitive * primitive, GLuint texture);

void PR_draw(Primitive * primitive, GLfloat width);
void PR_delete(Primitive * primitive);



