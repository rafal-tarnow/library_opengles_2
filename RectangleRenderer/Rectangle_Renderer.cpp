#include "Rectangle_Renderer.hpp"
#include "../../system_log.hpp"
#include "../Shader/ShadersSources/colour_shader_source.hpp"
#include "../Shader/ShadersSources/texture_shader_source.hpp"
#include "../ShaderManager/shader_manager.hpp"
#include "../TextureManager/texture_manager.hpp"
#include <iostream>

using namespace std;

static GLint compileShaders(const char *vertex_shader_source, const char *fragment_shader_source);

static GLfloat rectangle_vertices[] = {
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // TOP RIGHT VERTICES
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // BOTTOM RIGHT VERTICES
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // BOTTOM LEFT VERTICES
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f  // TOP RIGHT
};

static GLfloat rectangle_textCoord[] = {
    1.0f, 1.0f, // TOP RIGHT VERTICES
    1.0f, 0.0f, // BOTTOM RIGHT VERTICES
    0.0f, 0.0f, // BOTTOM LEFT VERTICES
    0.0f, 1.0f  // TOP RIGHT
};

static GLuint prepareVBO(GLsizeiptr size)
{
    GLuint vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

void updateVBO(GLuint vbo, const void *data, GLsizeiptr size)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DE_initRectangle_7(DE_Rectangle *rectangle, GLuint *textureId, GLfloat width, GLfloat height, GLfloat z)
{
    rectangle->texture_id = *textureId;
    rectangle->vbo_position = prepareVBO(sizeof(rectangle_vertices));

    GLfloat width_2 = width / 2.0f;
    GLfloat height_2 = height / 2.0f;

    // TOP RIGHT VERTICES
    rectangle_vertices[0] = width_2;
    rectangle_vertices[1] = height_2;
    rectangle_vertices[2] = z;

    rectangle_vertices[3] = 1.0f;
    rectangle_vertices[4] = 1.0f;

    // BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = width_2;
    rectangle_vertices[6] = -height_2;
    rectangle_vertices[7] = z;

    rectangle_vertices[8] = 1.0f;
    rectangle_vertices[9] = 0.0f;

    // BOTTOM LEFT VERTICES
    rectangle_vertices[10] = -width_2;
    rectangle_vertices[11] = -height_2;
    rectangle_vertices[12] = z;

    rectangle_vertices[13] = 0.0f;
    rectangle_vertices[14] = 0.0f;

    // TOP RIGHT
    rectangle_vertices[15] = -width_2;
    rectangle_vertices[16] = height_2;
    rectangle_vertices[17] = z;

    rectangle_vertices[18] = 0.0f;
    rectangle_vertices[19] = 1.0f;

    updateVBO(rectangle->vbo_position, rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_initRectangle_5(DE_Rectangle *rectangle, const char *textureFilename, GLfloat width, GLfloat height, GLfloat z)
{

    rectangle->texture_id = TextureManager::getInstance()->getTextureId(textureFilename);
    rectangle->vbo_position = prepareVBO(sizeof(rectangle_vertices));

    GLfloat width_2 = width / 2.0f;
    GLfloat height_2 = height / 2.0f;

    // TOP RIGHT VERTICES
    rectangle_vertices[0] = width_2;
    rectangle_vertices[1] = height_2;
    rectangle_vertices[2] = z;

    rectangle_vertices[3] = 1.0f;
    rectangle_vertices[4] = 1.0f;

    // BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = width_2;
    rectangle_vertices[6] = -height_2;
    rectangle_vertices[7] = z;

    rectangle_vertices[8] = 1.0f;
    rectangle_vertices[9] = 0.0f;

    // BOTTOM LEFT VERTICES
    rectangle_vertices[10] = -width_2;
    rectangle_vertices[11] = -height_2;
    rectangle_vertices[12] = z;

    rectangle_vertices[13] = 0.0f;
    rectangle_vertices[14] = 0.0f;

    //
    rectangle_vertices[15] = -width_2;
    rectangle_vertices[16] = height_2;
    rectangle_vertices[17] = z;

    rectangle_vertices[18] = 0.0f;
    rectangle_vertices[19] = 1.0f;

    updateVBO(rectangle->vbo_position, rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_initRectangle_6(DE_Rectangle *rectangle, GLuint textureId, glm::vec3 *verticles, glm::vec2 *texCoords)
{
    rectangle->texture_id = textureId;
    rectangle->vbo_position = prepareVBO(sizeof(rectangle_vertices));

    // TOP RIGHT VERTICES
    rectangle_vertices[0] = verticles[0].x;
    rectangle_vertices[1] = verticles[0].y;
    rectangle_vertices[2] = verticles[0].z;

    rectangle_vertices[3] = texCoords[0].x;
    rectangle_vertices[4] = texCoords[0].y;

    // BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = verticles[1].x;
    rectangle_vertices[6] = verticles[1].y;
    rectangle_vertices[7] = verticles[1].z;

    rectangle_vertices[8] = texCoords[1].x;
    rectangle_vertices[9] = texCoords[1].y;

    // BOTTOM LEFT VERTICES
    rectangle_vertices[10] = verticles[2].x;
    rectangle_vertices[11] = verticles[2].y;
    rectangle_vertices[12] = verticles[2].z;

    rectangle_vertices[13] = texCoords[2].x;
    rectangle_vertices[14] = texCoords[2].y;

    //
    rectangle_vertices[15] = verticles[3].x;
    rectangle_vertices[16] = verticles[3].y;
    rectangle_vertices[17] = verticles[3].z;

    rectangle_vertices[18] = texCoords[3].x;
    rectangle_vertices[19] = texCoords[3].y;

    updateVBO(rectangle->vbo_position, rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_initRectangle_8(DE_Rectangle *rectangle, vector<glm::vec3> &verticles)
{
    rectangle->texture_id = 0;
    rectangle->vbo_position = prepareVBO(sizeof(rectangle_vertices));

    // TOP RIGHT VERTICES
    rectangle_vertices[0] = verticles[0].x;
    rectangle_vertices[1] = verticles[0].y;
    rectangle_vertices[2] = verticles[0].z;


    // BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = verticles[1].x;
    rectangle_vertices[6] = verticles[1].y;
    rectangle_vertices[7] = verticles[1].z;

    // BOTTOM LEFT VERTICES
    rectangle_vertices[10] = verticles[2].x;
    rectangle_vertices[11] = verticles[2].y;
    rectangle_vertices[12] = verticles[2].z;

    //
    rectangle_vertices[15] = verticles[3].x;
    rectangle_vertices[16] = verticles[3].y;
    rectangle_vertices[17] = verticles[3].z;

    updateVBO(rectangle->vbo_position, rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_setVerticles(DE_Rectangle *rectangle, vector<glm::vec3> &verticles)
{
    updateVBO(rectangle->vbo_position, verticles.data(), verticles.size() * sizeof(glm::vec3));
}

void DE_initRectangle_3(DE_Rectangle *rectangle, GLuint textureId, glm::vec2 dimm)
{

    rectangle->texture_id = textureId;
    rectangle->vbo_position = prepareVBO(sizeof(rectangle_vertices));

    GLfloat width_2 = dimm.x / 2.0f;
    GLfloat height_2 = dimm.y / 2.0f;

    // TOP RIGHT VERTICES
    rectangle_vertices[0] = width_2;
    rectangle_vertices[1] = height_2;
    rectangle_vertices[2] = 0;

    rectangle_vertices[3] = 1.0f;
    rectangle_vertices[4] = 1.0f;

    // BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = width_2;
    rectangle_vertices[6] = -height_2;
    rectangle_vertices[7] = 0;

    rectangle_vertices[8] = 1.0f;
    rectangle_vertices[9] = 0.0f;

    // BOTTOM LEFT VERTICES
    rectangle_vertices[10] = -width_2;
    rectangle_vertices[11] = -height_2;
    rectangle_vertices[12] = 0;

    rectangle_vertices[13] = 0.0f;
    rectangle_vertices[14] = 0.0f;
    //
    rectangle_vertices[15] = -width_2;
    rectangle_vertices[16] = height_2;
    rectangle_vertices[17] = 0;

    rectangle_vertices[18] = 0.0f;
    rectangle_vertices[19] = 1.0f;

    updateVBO(rectangle->vbo_position, rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_setDimm(DE_Rectangle *rectangle, glm::vec2 dimm)
{
    rectangle->dimm = dimm;

    GLfloat width_2 = dimm.x / 2.0f;
    GLfloat height_2 = dimm.y / 2.0f;

    // TOP RIGHT VERTICES
    rectangle_vertices[0] = width_2;
    rectangle_vertices[1] = height_2;
    rectangle_vertices[2] = 0;

    rectangle_vertices[3] = 1.0f;
    rectangle_vertices[4] = 1.0f;

    // BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = width_2;
    rectangle_vertices[6] = -height_2;
    rectangle_vertices[7] = 0;

    rectangle_vertices[8] = 1.0f;
    rectangle_vertices[9] = 0.0f;

    // BOTTOM LEFT VERTICES
    rectangle_vertices[10] = -width_2;
    rectangle_vertices[11] = -height_2;
    rectangle_vertices[12] = 0;

    rectangle_vertices[13] = 0.0f;
    rectangle_vertices[14] = 0.0f;
    //
    rectangle_vertices[15] = -width_2;
    rectangle_vertices[16] = height_2;
    rectangle_vertices[17] = 0;

    rectangle_vertices[18] = 0.0f;
    rectangle_vertices[19] = 1.0f;

    updateVBO(rectangle->vbo_position, rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_setColour(DE_Rectangle *rectangle, glm::vec4 colour)
{
    rectangle->colour = colour;
}

void DE_setShader(DE_Rectangle *rectangle, Shader_m *shader)
{
    if (shader)
    {
        rectangle->shader = shader;

        rectangle->position_location = rectangle->shader->getAttribLocation("position");
        rectangle->texCoord_attrib_location = rectangle->shader->getAttribLocation("texCoord");
        rectangle->textureUnitLocation = rectangle->shader->getUniformLocation("textureUnit");
        rectangle->projectionMatrixLocation = rectangle->shader->getUniformLocation("projection");
        rectangle->viewMatrixLocation = rectangle->shader->getUniformLocation("view");
        rectangle->modelMatrixLocation = rectangle->shader->getUniformLocation("model");
        rectangle->colourLocation = rectangle->shader->getUniformLocation("colour");
    }
}

void DE_setModel(DE_Rectangle *rectancle, glm::mat4 model)
{
    rectancle->model = model;
}

void DE_drawRectangle(DE_Rectangle *rectangle, GLenum mode, Shader_m * shader)
{

    if (rectangle->texture_id != 0)
    {
        if (rectangle->shader)
        {
            rectangle->shader->use();
            rectangle->shader->setMat4(rectangle->projectionMatrixLocation, rectangle->projection);
            rectangle->shader->setMat4(rectangle->viewMatrixLocation, rectangle->view);
            rectangle->shader->setMat4(rectangle->modelMatrixLocation, rectangle->model);


            GLuint texture_unit = 0;

            glActiveTexture(GL_TEXTURE0 + texture_unit);
            glBindTexture(GL_TEXTURE_2D, rectangle->texture_id);
            glUniform1i(rectangle->textureUnitLocation, texture_unit);

            glBindBuffer(GL_ARRAY_BUFFER, rectangle->vbo_position);
            {
                glVertexAttribPointer(rectangle->position_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
                glEnableVertexAttribArray(rectangle->position_location);

                glVertexAttribPointer(rectangle->texCoord_attrib_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
                glEnableVertexAttribArray(rectangle->texCoord_attrib_location);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(mode, 0, 4);

        }
        else if (shader)
        {

            GLuint texture_unit = 0;

            glActiveTexture(GL_TEXTURE0 + texture_unit);
            glBindTexture(GL_TEXTURE_2D, rectangle->texture_id);
            glUniform1i(shader->getUniformLocation("textureUnit"), texture_unit);

            glBindBuffer(GL_ARRAY_BUFFER, rectangle->vbo_position);
            {
                glVertexAttribPointer(shader->getAttribLocation("position"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
                glEnableVertexAttribArray(shader->getAttribLocation("position"));

                glVertexAttribPointer(shader->getAttribLocation("texCoord"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
                glEnableVertexAttribArray(shader->getAttribLocation("texCoord"));
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(mode, 0, 4);

        }
    }
    else
    {
        if (rectangle->shader)
        {
            //rectangle->shader->use();
            //rectangle->shader->setMat4(rectangle->projectionMatrixLocation, rectangle->projection);
            //rectangle->shader->setMat4(rectangle->viewMatrixLocation, rectangle->view);
            //rectangle->shader->setMat4(rectangle->modelMatrixLocation, rectangle->model);
            //rectangle->shader->setVec4(rectangle->colourLocation, rectangle->colour);

            glBindBuffer(GL_ARRAY_BUFFER, rectangle->vbo_position);
            {
                glVertexAttribPointer(rectangle->position_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
                glEnableVertexAttribArray(rectangle->position_location);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(mode, 0, 4);
        }
        else if (shader)
        {
            //rectangle->shader->use();
            //rectangle->shader->setMat4(rectangle->projectionMatrixLocation, rectangle->projection);
            //rectangle->shader->setMat4(rectangle->viewMatrixLocation, rectangle->view);
            //rectangle->shader->setMat4(rectangle->modelMatrixLocation, rectangle->model);
            //rectangle->shader->setVec4(rectangle->colourLocation, rectangle->colour);

            glBindBuffer(GL_ARRAY_BUFFER, rectangle->vbo_position);
            {
                glVertexAttribPointer(shader->getAttribLocation("position"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
                glEnableVertexAttribArray(shader->getAttribLocation("position"));
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(mode, 0, 4);
        }

    }
}

void DE_deleteRectangle(DE_Rectangle *rectangle)
{
    glDeleteBuffers(1, &(rectangle->vbo_position));
}
