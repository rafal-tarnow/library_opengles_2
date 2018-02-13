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
} LS_LineStrip;


void LS_initLineStrip(LS_LineStrip * lineStrip, float * verticlesTable, int tableSize);
void LS_drawLineStrip(LS_LineStrip * lineStrip, GLfloat width);
void LS_deleteLineStrip(LS_LineStrip * lineStrip);



