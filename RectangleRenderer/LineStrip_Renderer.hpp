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


void LS_init(LS_LineStrip * lineStrip, float * verticlesTable, int tableSize);
void LS_init(LS_LineStrip * lineStrip, glm::vec3 * verticlesTable, int tableSize);
void LS_updateData(LS_LineStrip * lineStrip, glm::vec3 * verticlesTable, int tableSize);
void LS_draw(LS_LineStrip * lineStrip, GLfloat width);
void LS_delete(LS_LineStrip * lineStrip);



