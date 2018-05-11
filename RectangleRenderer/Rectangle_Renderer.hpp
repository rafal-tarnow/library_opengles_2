#pragma once

#include <GLES2/gl2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


typedef struct{
    GLuint vbo_id;
    GLuint texture_id;
    glm::vec4 colour;
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
} DE_Rectangle;

void DE_initRectangle_1(DE_Rectangle * rectangle, GLuint textureId, GLfloat x_top_left = -0.5f, GLfloat y_top_left = 0.5f, GLfloat x_bottom_right = 0.5f, GLfloat y_bottom_right = -0.5f, GLfloat z = 0.0f);
void DE_initRectangle(DE_Rectangle * rectangle, const char * textureFilename, GLfloat x_top_left, GLfloat y_top_left, GLfloat x_bottom_right, GLfloat y_bottom_right, GLfloat z = 0.0f);

void DE_initRectangle(DE_Rectangle * rectangle, const char * textureFilename, glm::vec3 position, glm::vec2 dimm);
void DE_initRectangle(DE_Rectangle * rectangle, GLuint textureId, glm::vec3 position, glm::vec2 dimm);
void DE_initRectangle(DE_Rectangle * rectangle, const char * textureFilename, GLfloat width, GLfloat height, GLfloat z = 0.0f);
void DE_initRectangle(DE_Rectangle * rectangle, GLuint * textureId, GLfloat width, GLfloat height, GLfloat z = 0.0f);
void DE_initRectangle(DE_Rectangle * rectangle, glm::vec4 colour, glm::vec3 position, glm::vec2 dimm);

void DE_drawRectangle(DE_Rectangle * rectangle);

void DE_deleteRectangle(DE_Rectangle * rectangle);



