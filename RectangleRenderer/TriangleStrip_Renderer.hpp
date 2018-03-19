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
    GLuint texture_id;
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
} TS_TriangleStrip;


void TS_initTriangleStrip(TS_TriangleStrip * triangleStrip, float * verticlesTable, int tableSize, glm::vec4 color);
void TS_initTriangleStrip(TS_TriangleStrip * triangleStrip, glm::vec3 * verticlesTable, int tableSize, glm::vec4 color);
void TS_initTriangleStrip(TS_TriangleStrip * triangleStrip, glm::vec3 * verticlesTable, int tableSize, GLuint textureId);
void TS_drawTriangleStrip(TS_TriangleStrip * triangleStrip);
void TS_deleteTriangleStrip(TS_TriangleStrip * triangleStrip);


