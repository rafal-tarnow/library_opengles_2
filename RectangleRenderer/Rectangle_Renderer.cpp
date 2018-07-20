#include "Rectangle_Renderer.hpp"
#include "../TextureManager/texture_manager.hpp"
#include <iostream>
#include "../../system_log.hpp"

using namespace std;

static GLint compileShaders(const char *vertex_shader_source, const char *fragment_shader_source);

static const GLchar* texture_vertex_shader_source =
        "#version 100                           \n"
        "//Rectangle_Renderer vertex shader     \n"
        "attribute vec3 position;               \n"
        "attribute vec2 texCoord;               \n"
        "varying vec2 v_TexCoordinate;          \n"
        "                                       \n"
        "uniform mat4 model;                    \n"
        "uniform mat4 view;                     \n"
        "uniform mat4 projection;               \n"
        "                                       \n"
        "void main() {                          \n"
        "   gl_Position =  projection * view * model * vec4(position, 1.0);  \n"
        "   v_TexCoordinate = texCoord;         \n"
        "}                                      \n";


static const GLchar* texture_fragment_shader_source =
        "#version 100                                               \n"
        "//Rectangle_Renderer fragmet shader                        \n"
        "precision mediump float;                                   \n"
        "varying vec2 v_TexCoordinate;                              \n"
        "uniform sampler2D textureUnit;                             \n"
        "                                                           \n"
        "void main() {                                              \n"
        "   gl_FragColor = texture2D(textureUnit,v_TexCoordinate);    \n"
        "}                                                          \n";


static const GLchar* colour_vertex_shader_source =
        "#version 100                           \n"
        "//Rectangle_Renderer vertex shader     \n"
        "attribute vec3 position;               \n"
        "attribute vec2 texCoord;               \n"
        "                                       \n"
        "uniform mat4 model;                    \n"
        "uniform mat4 view;                     \n"
        "uniform mat4 projection;               \n"
        "                                       \n"
        "void main() {                          \n"
        "   gl_Position =  projection * view * model * vec4(position, 1.0);  \n"
        "}                                      \n";


static const GLchar* colour_fragment_shader_source =
        "#version 100                                               \n"
        "//Rectangle_Renderer fragmet shader                        \n"
        "precision mediump float;                                   \n"
        "                                                           \n"
        "void main() {                                              \n"
        "   gl_FragColor = vec4(1.0,0.0,0.0,1.0);    \n"
        "}                                                          \n";



static GLfloat rectangle_vertices[] = {
    1.0f,  1.0f, 0.0f,      1.0f, 1.0f,  //TOP RIGHT VERTICES
    1.0f, -1.0f, 0.0f,      1.0f, 0.0f,  //BOTTOM RIGHT VERTICES
    -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,  //BOTTOM LEFT VERTICES
    -1.0f, 1.0f, 0.0f,      0.0f, 1.0f  //TOP RIGHT
};

static GLuint prepareVBO(GLsizeiptr size){
    GLuint vbo;

    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

void updateVBO(GLuint vbo, const GLfloat * data, GLsizeiptr size)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


//TEXTURE PROGRAM LOCATIONS
static GLint position_location_tex;
static GLint texCoord_attrib_location_tex;
static GLint textureUnitLocation_tex;
static GLint projectionMatrixLocation_tex;
static GLint viewMatrixLocation_tex;
static GLint modelMatrixLocation_tex;

//COLOUR PROGRAM LOCATIONS
static GLint position_location_col;
static GLint projectionMatrixLocation_col;
static GLint viewMatrixLocation_col;
static GLint modelMatrixLocation_col;

static GLuint texture_shader_program;
static GLuint colour_shader_program;
static int shaderInited = 0;

void initShader(){
    if(shaderInited == 0)
    {
        //TEXTURE PROGRAM
        {
            texture_shader_program = compileShaders(texture_vertex_shader_source, texture_fragment_shader_source);

            position_location_tex = glGetAttribLocation(texture_shader_program, "position");
            texCoord_attrib_location_tex = glGetAttribLocation(texture_shader_program,"texCoord");
            textureUnitLocation_tex = glGetUniformLocation (texture_shader_program, "textureUnit" );

            projectionMatrixLocation_tex = glGetUniformLocation(texture_shader_program, "projection");
            viewMatrixLocation_tex = glGetUniformLocation(texture_shader_program, "view");
            modelMatrixLocation_tex = glGetUniformLocation(texture_shader_program, "model");
        }

        //COLOUR PROGRAM
        {
            colour_shader_program = compileShaders(colour_vertex_shader_source, colour_fragment_shader_source);

            position_location_col = glGetAttribLocation(colour_shader_program, "position");

            projectionMatrixLocation_col = glGetUniformLocation(colour_shader_program, "projection");
            viewMatrixLocation_col = glGetUniformLocation(colour_shader_program, "view");
            modelMatrixLocation_col = glGetUniformLocation(colour_shader_program, "model");
        }

        shaderInited = 1;
    }
    else
    {
        cout << "[WARNING] shader arlady compiled" << endl;
    }
}

void DE_initRectangle(DE_Rectangle * rectangle, GLuint  * textureId, GLfloat width, GLfloat height, GLfloat z)
{
    if(shaderInited == 0)
    {
        initShader();
    }

    rectangle->texture_id = *textureId;
    rectangle->vbo_id = prepareVBO(sizeof(rectangle_vertices));


    GLfloat width_2 = width/2.0f;
    GLfloat height_2 = height/2.0f;

    //TOP RIGHT VERTICES
    rectangle_vertices[0] = width_2;
    rectangle_vertices[1] = height_2;
    rectangle_vertices[2] = z;

    rectangle_vertices[3] = 1.0f;
    rectangle_vertices[4] = 1.0f;

    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = width_2;
    rectangle_vertices[6] = -height_2;
    rectangle_vertices[7] = z;

    rectangle_vertices[8] = 1.0f;
    rectangle_vertices[9] = 0.0f;

    //BOTTOM LEFT VERTICES
    rectangle_vertices[10] = -width_2;
    rectangle_vertices[11] = -height_2;
    rectangle_vertices[12] = z;

    rectangle_vertices[13] = 0.0f;
    rectangle_vertices[14] = 0.0f;

    //TOP RIGHT
    rectangle_vertices[15] = -width_2;
    rectangle_vertices[16] = height_2;
    rectangle_vertices[17] = z;

    rectangle_vertices[18] = 0.0f;
    rectangle_vertices[19] = 1.0f;

    updateVBO(rectangle->vbo_id, rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_initRectangle(DE_Rectangle * rectangle, const char * textureFilename, GLfloat width, GLfloat height, GLfloat z)
{
    if(shaderInited == 0)
    {
        initShader();
    }


    rectangle->texture_id = TextureManager::getTextureId(textureFilename);
    rectangle->vbo_id = prepareVBO(sizeof(rectangle_vertices));


    GLfloat width_2 = width/2.0f;
    GLfloat height_2 = height/2.0f;

    //TOP RIGHT VERTICES
    rectangle_vertices[0] = width_2;
    rectangle_vertices[1] = height_2;
    rectangle_vertices[2] = z;

    rectangle_vertices[3] = 1.0f;
    rectangle_vertices[4] = 1.0f;

    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = width_2;
    rectangle_vertices[6] = -height_2;
    rectangle_vertices[7] = z;

    rectangle_vertices[8] = 1.0f;
    rectangle_vertices[9] = 0.0f;

    //BOTTOM LEFT VERTICES
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

    updateVBO(rectangle->vbo_id, rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_initRectangle(DE_Rectangle * rectangle, GLuint textureId, glm::vec3 * verticles, glm::vec2 * texCoords)
{
    if(shaderInited == 0)
    {
        initShader();
    }


    rectangle->texture_id = textureId;
    rectangle->vbo_id = prepareVBO(sizeof(rectangle_vertices));



    //TOP RIGHT VERTICES
    rectangle_vertices[0] = verticles[0].x;
    rectangle_vertices[1] = verticles[0].y;
    rectangle_vertices[2] = verticles[0].z;

    rectangle_vertices[3] = texCoords[0].x;
    rectangle_vertices[4] = texCoords[0].y;

    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = verticles[1].x;
    rectangle_vertices[6] = verticles[1].y;
    rectangle_vertices[7] = verticles[1].z;

    rectangle_vertices[8] = texCoords[1].x;
    rectangle_vertices[9] = texCoords[1].y;

    //BOTTOM LEFT VERTICES
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

    updateVBO(rectangle->vbo_id, rectangle_vertices, sizeof(rectangle_vertices));

}

void DE_initRectangle(DE_Rectangle * rectangle, glm::vec4 colour, glm::vec3 position, glm::vec2 dimm)
{
    if(shaderInited == 0)
    {
        initShader();
    }


    rectangle->position = position;
    rectangle->dimm = dimm;
    rectangle->texture_id = 0;
    rectangle->colour = colour;
    rectangle->vbo_id = prepareVBO(sizeof(rectangle_vertices));


    GLfloat width_2 = dimm.x/2.0f;
    GLfloat height_2 = dimm.y/2.0f;
    GLfloat z = 0;

    //TOP RIGHT VERTICES
    rectangle_vertices[0] = width_2 + position.x;
    rectangle_vertices[1] = height_2 + position.y;
    rectangle_vertices[2] = position.z;

    rectangle_vertices[3] = 1.0f;
    rectangle_vertices[4] = 1.0f;

    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = width_2 + position.x;
    rectangle_vertices[6] = -height_2 + position.y;
    rectangle_vertices[7] = position.z;

    rectangle_vertices[8] = 1.0f;
    rectangle_vertices[9] = 0.0f;

    //BOTTOM LEFT VERTICES
    rectangle_vertices[10] = -width_2 + position.x;
    rectangle_vertices[11] = -height_2 + position.y;
    rectangle_vertices[12] = position.z;

    rectangle_vertices[13] = 0.0f;
    rectangle_vertices[14] = 0.0f;

    //
    rectangle_vertices[15] = -width_2 + position.x;
    rectangle_vertices[16] = height_2 + position.y;
    rectangle_vertices[17] = position.z;

    rectangle_vertices[18] = 0.0f;
    rectangle_vertices[19] = 1.0f;

    updateVBO(rectangle->vbo_id, rectangle_vertices, sizeof(rectangle_vertices));

}

void DE_initRectangle(DE_Rectangle * rectangle, const char * textureFilename, glm::vec3 position, glm::vec2 dimm)
{
    if(shaderInited == 0)
    {
        initShader();
    }


    rectangle->texture_id = TextureManager::getTextureId(textureFilename);
    rectangle->vbo_id = prepareVBO(sizeof(rectangle_vertices));


    GLfloat width_2 = dimm.x/2.0f;
    GLfloat height_2 = dimm.y/2.0f;

    //TOP RIGHT VERTICES
    rectangle_vertices[0] = width_2 + position.x;
    rectangle_vertices[1] = height_2 + position.y;
    rectangle_vertices[2] = position.z;

    rectangle_vertices[3] = 1.0f;
    rectangle_vertices[4] = 1.0f;

    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = width_2 + position.x;
    rectangle_vertices[6] = -height_2 + position.y;
    rectangle_vertices[7] = position.z;

    rectangle_vertices[8] = 1.0f;
    rectangle_vertices[9] = 0.0f;

    //BOTTOM LEFT VERTICES
    rectangle_vertices[10] = -width_2 + position.x;
    rectangle_vertices[11] = -height_2 + position.y;
    rectangle_vertices[12] = position.z;

    rectangle_vertices[13] = 0.0f;
    rectangle_vertices[14] = 0.0f;
    //
    rectangle_vertices[15] = -width_2 + position.x;
    rectangle_vertices[16] = height_2 + position.y;
    rectangle_vertices[17] = position.z;

    rectangle_vertices[18] = 0.0f;
    rectangle_vertices[19] = 1.0f;

    updateVBO(rectangle->vbo_id, rectangle_vertices, sizeof(rectangle_vertices));  
}

void DE_setDimm(DE_Rectangle * rectangle, glm::vec2 dimm)
{
    rectangle->dimm = dimm;

    GLfloat width_2 = dimm.x/2.0f;
    GLfloat height_2 = dimm.y/2.0f;

    //TOP RIGHT VERTICES
    rectangle_vertices[0] = width_2 + rectangle->position.x;
    rectangle_vertices[1] = height_2 + rectangle->position.y;
    rectangle_vertices[2] = rectangle->position.z;
    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = width_2 + rectangle->position.x;
    rectangle_vertices[6] = -height_2 + rectangle->position.y;
    rectangle_vertices[7] = rectangle->position.z;
    //BOTTOM LEFT VERTICES
    rectangle_vertices[10] = -width_2 + rectangle->position.x;
    rectangle_vertices[11] = -height_2 + rectangle->position.y;
    rectangle_vertices[12] = rectangle->position.z;
    //
    rectangle_vertices[15] = -width_2 + rectangle->position.x;
    rectangle_vertices[16] = height_2 + rectangle->position.y;
    rectangle_vertices[17] = rectangle->position.z;

    updateVBO(rectangle->vbo_id, rectangle_vertices, sizeof(rectangle_vertices));
}


void DE_initRectangle(DE_Rectangle * rectangle, GLuint textureId, glm::vec2 dimm)
{
    if(shaderInited == 0)
    {
        initShader();
    }



    rectangle->texture_id = textureId;
    rectangle->vbo_id = prepareVBO(sizeof(rectangle_vertices));

    GLfloat width_2 = dimm.x/2.0f;
    GLfloat height_2 = dimm.y/2.0f;
    GLfloat z = 0.0f;

    //TOP RIGHT VERTICES
    rectangle_vertices[0] = width_2;
    rectangle_vertices[1] = height_2;
    rectangle_vertices[2] = z;

    rectangle_vertices[3] = 1.0f;
    rectangle_vertices[4] = 1.0f;

    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = width_2;
    rectangle_vertices[6] = -height_2;
    rectangle_vertices[7] = z;

    rectangle_vertices[8] = 1.0f;
    rectangle_vertices[9] = 0.0f;

    //BOTTOM LEFT VERTICES
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

    updateVBO(rectangle->vbo_id, rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_initRectangle(DE_Rectangle * rectangle, const char * textureFilename, GLfloat x_top_left, GLfloat y_top_left, GLfloat x_bottom_right, GLfloat y_bottom_right, GLfloat z)
{
    if(shaderInited == 0)
    {
        initShader();
    }



    rectangle->texture_id = TextureManager::getTextureId(textureFilename);
    rectangle->vbo_id = prepareVBO(sizeof(rectangle_vertices));

    //TOP RIGHT VERTICES
    rectangle_vertices[0] = x_bottom_right;
    rectangle_vertices[1] = y_top_left;
    rectangle_vertices[2] = z;

    rectangle_vertices[3] = 1.0f;
    rectangle_vertices[4] = 1.0f;

    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = x_bottom_right;
    rectangle_vertices[6] = y_bottom_right;
    rectangle_vertices[7] = z;

    rectangle_vertices[8] = 1.0f;
    rectangle_vertices[9] = 0.0f;

    //BOTTOM LEFT VERTICES
    rectangle_vertices[10] = x_top_left;
    rectangle_vertices[11] = y_bottom_right;
    rectangle_vertices[12] = z;

    rectangle_vertices[13] = 0.0f;
    rectangle_vertices[14] = 0.0f;

    //TOP LEFT VERTICES
    rectangle_vertices[15] = x_top_left;
    rectangle_vertices[16] = y_top_left;
    rectangle_vertices[17] = z;

    rectangle_vertices[18] = 0.0f;
    rectangle_vertices[19] = 1.0f;

    updateVBO(rectangle->vbo_id, rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_initRectangle_1(DE_Rectangle * rectangle, GLuint textureId, GLfloat x_top_left, GLfloat y_top_left, GLfloat x_bottom_right, GLfloat y_bottom_right, GLfloat z)
{
    if(shaderInited == 0)
    {
        initShader();
    }


    rectangle->texture_id = textureId;
    rectangle->vbo_id = prepareVBO(sizeof(rectangle_vertices));

    //TOP RIGHT VERTICES
    rectangle_vertices[0] = x_bottom_right;
    rectangle_vertices[1] = y_top_left;
    rectangle_vertices[2] = z;

    rectangle_vertices[3] = 1.0f;
    rectangle_vertices[4] = 1.0f;

    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = x_bottom_right;
    rectangle_vertices[6] = y_bottom_right;
    rectangle_vertices[7] = z;

    rectangle_vertices[8] = 1.0f;
    rectangle_vertices[9] = 0.0f;

    //BOTTOM LEFT VERTICES
    rectangle_vertices[10] = x_top_left;
    rectangle_vertices[11] = y_bottom_right;
    rectangle_vertices[12] = z;

    rectangle_vertices[13] = 0.0f;
    rectangle_vertices[14] = 0.0f;

    //TOP LEFT VERTICES
    rectangle_vertices[15] = x_top_left;
    rectangle_vertices[16] = y_top_left;
    rectangle_vertices[17] = z;

    rectangle_vertices[18] = 0.0f;
    rectangle_vertices[19] = 1.0f;

    updateVBO(rectangle->vbo_id, rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_drawRectangle(DE_Rectangle * rectangle){
    if(rectangle->texture_id != 0)
    {
        glUseProgram(texture_shader_program);
        {
            glUniformMatrix4fv(projectionMatrixLocation_tex, 1, GL_FALSE, glm::value_ptr(rectangle->projection));
            glUniformMatrix4fv(viewMatrixLocation_tex, 1, GL_FALSE, glm::value_ptr(rectangle->view));
            glUniformMatrix4fv(modelMatrixLocation_tex, 1, GL_FALSE, glm::value_ptr(rectangle->model));

            GLuint texture_unit = 0;

            glActiveTexture(GL_TEXTURE0 + texture_unit);
            glBindTexture(GL_TEXTURE_2D, rectangle->texture_id);
            glUniform1i(textureUnitLocation_tex, texture_unit);


            glBindBuffer(GL_ARRAY_BUFFER, rectangle->vbo_id);
            {
                glVertexAttribPointer(position_location_tex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
                glEnableVertexAttribArray(position_location_tex);

                glVertexAttribPointer(texCoord_attrib_location_tex, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
                glEnableVertexAttribArray(texCoord_attrib_location_tex);

                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        glUseProgram(0);
    }else
    {
        glUseProgram(colour_shader_program);
        {
            glUniformMatrix4fv(projectionMatrixLocation_col, 1, GL_FALSE, glm::value_ptr(rectangle->projection));
            glUniformMatrix4fv(viewMatrixLocation_col, 1, GL_FALSE, glm::value_ptr(rectangle->view));
            glUniformMatrix4fv(modelMatrixLocation_col, 1, GL_FALSE, glm::value_ptr(rectangle->model));

            glBindBuffer(GL_ARRAY_BUFFER, rectangle->vbo_id);
            {
                glVertexAttribPointer(position_location_col, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
                glEnableVertexAttribArray(position_location_col);

                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        glUseProgram(0);
    }
}

void DE_deleteRectangle(DE_Rectangle * rectangle){
    glDeleteBuffers(1, &(rectangle->vbo_id));
}


GLint compileShaders(const char *vertex_shader_source, const char *fragment_shader_source) {
    enum Consts {INFOLOG_LEN = 512};
    GLchar infoLog[INFOLOG_LEN];
    GLint fragment_shader;
    GLint shader_program;
    GLint success;
    GLint vertex_shader;

    /* Vertex shader */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, INFOLOG_LEN, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n%s\n",vertex_shader_source, infoLog);
        exit(-1);
    }

    /* Fragment shader */
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, INFOLOG_LEN, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n%s\n",fragment_shader_source, infoLog);
        exit(-1);
    }

    /* Link shaders */
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, INFOLOG_LEN, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
        exit(-1);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_program;
}
