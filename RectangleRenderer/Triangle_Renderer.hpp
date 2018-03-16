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
} TR_Triangle;


void TR_init(TR_Triangle * lineStrip, float * verticlesTable);
void TR_init(TR_Triangle * lineStrip, glm::vec3 * verticlesTable);
//void TR_updateData(TR_Triangle * lineStrip, glm::vec3 * verticlesTable, int tableSize);
void TR_draw(TR_Triangle * lineStrip, GLfloat width);
void TR_delete(TR_Triangle * lineStrip);



