#pragma once

#include "../opengl_includes.hpp"
#include "../Shader/shader_m.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using std::vector;


typedef struct{
    GLuint vbo_id;
    GLuint texture_id;
    glm::vec4 colour;
    glm::vec2 dimm;
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    Shader_m * shader = nullptr;

     GLint position_location;
     GLint texCoord_attrib_location;
     GLint textureUnitLocation;
     GLint projectionMatrixLocation;
     GLint viewMatrixLocation;
     GLint modelMatrixLocation;

} DE_Rectangle;


void DE_initRectangle_3(DE_Rectangle * rectangle, GLuint textureId, glm::vec2 dimm);
void DE_initRectangle_5(DE_Rectangle * rectangle, const char * textureFilename, GLfloat width, GLfloat height, GLfloat z = 0.0f);
void DE_initRectangle_6(DE_Rectangle * rectangle, GLuint textureId, glm::vec3 * verticles, glm::vec2 * texCoords);
void DE_initRectangle_8(DE_Rectangle * rectangle, vector<glm::vec3> & verticles);
void DE_initRectangle_7(DE_Rectangle * rectangle, GLuint * textureId, GLfloat width, GLfloat height, GLfloat z = 0.0f);

void DE_setVerticles(DE_Rectangle * rectangle, vector<glm::vec3> & verticles);
void DE_setShader(DE_Rectangle * rectangle, Shader_m * shader);
void DE_setModel(DE_Rectangle * rectancle, glm::mat4 model);
void DE_setDimm(DE_Rectangle * rectangle, glm::vec2 dimm);

void DE_drawRectangle(DE_Rectangle * rectangle, GLenum mode = GL_TRIANGLE_FAN);

void DE_deleteRectangle(DE_Rectangle * rectangle);



