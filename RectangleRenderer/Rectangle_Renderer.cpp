#include "Rectangle_Renderer.hpp"
#include "../TextureManager/texture_manager.hpp"
#include <iostream>
#include "../../system_log.hpp"

using namespace std;

static GLint compileShaders(const char *vertex_shader_source, const char *fragment_shader_source);

static const GLchar* vertex_shader_source =
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


static const GLchar* fragment_shader_source =
        "#version 100                                               \n"
        "//Rectangle_Renderer fragmet shader                        \n"
        "precision mediump float;                                   \n"
        "varying vec2 v_TexCoordinate;                              \n"
        "uniform sampler2D textureUnit;                             \n"
        "                                                           \n"
        "void main() {                                              \n"
        "   vec4 color = texture2D(textureUnit,v_TexCoordinate);    \n"
        "   //if (color.a < 0.9)                                    \n"
        "   //   discard;                                           \n"
        "   //color.a = 0.5;                                          \n"
        "   gl_FragColor = color;                                   \n"
        "}                                                          \n";

static GLfloat rectangle_vertices[] = {
    1.0f,  1.0f, 0.0f,      1.0f, 1.0f,
    1.0f, -1.0f, 0.0f,      1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,      0.0f, 1.0f
};

static GLuint prepareVBO(const GLfloat * data, GLsizeiptr size){
    GLuint vbo;

    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

static GLint projectionMatrixLocation;
static GLint viewMatrixLocation;
static GLint modelMatrixLocation;
static GLint position_location;
static GLint texCoord_attrib_location;
static GLint textureUnitLocation;
static GLuint shader_program;
static int shaderInited = 0;

GLuint DE_initShader(){
    if(shaderInited == 0)
    {
        shader_program = compileShaders(vertex_shader_source, fragment_shader_source);

        position_location = glGetAttribLocation(shader_program, "position");
        texCoord_attrib_location = glGetAttribLocation(shader_program,"texCoord");
        textureUnitLocation = glGetUniformLocation (shader_program, "textureUnit" );

        projectionMatrixLocation = glGetUniformLocation(shader_program, "projection");
        viewMatrixLocation = glGetUniformLocation(shader_program, "view");
        modelMatrixLocation = glGetUniformLocation(shader_program, "model");

        shaderInited = 1;
    }
    else
    {
        cout << "[WARNING] shader arlady compiled" << endl;
    }
    return shader_program;
}

void DE_initRectangle(DE_Rectangle * rectangle, GLuint  * textureId, GLfloat width, GLfloat height, GLfloat z)
{
    if(shaderInited == 0)
    {
        LOGD("ERROR shader not inited");
        exit(-1);
    }

    GLfloat width_2 = width/2.0f;
    GLfloat height_2 = height/2.0f;

    //TOP RIGHT VERTICES
    rectangle_vertices[0] = width_2;
    rectangle_vertices[1] = height_2;
    rectangle_vertices[2] = z;
    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = width_2;
    rectangle_vertices[6] = -height_2;
    rectangle_vertices[7] = z;
    //BOTTOM LEFT VERTICES
    rectangle_vertices[10] = -width_2;
    rectangle_vertices[11] = -height_2;
    rectangle_vertices[12] = z;
    //
    rectangle_vertices[15] = -width_2;
    rectangle_vertices[16] = height_2;
    rectangle_vertices[17] = z;

    rectangle->texture_id = *textureId;
    rectangle->vbo_id = prepareVBO(rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_initRectangle(DE_Rectangle * rectangle, const char * textureFilename, GLfloat width, GLfloat height, GLfloat z)
{
    if(shaderInited == 0)
    {
        cout << "[ERROR] DE_initRectangle shader not inited" << endl;
        exit(-1);
    }

    GLfloat width_2 = width/2.0f;
    GLfloat height_2 = height/2.0f;

    //TOP RIGHT VERTICES
    rectangle_vertices[0] = width_2;
    rectangle_vertices[1] = height_2;
    rectangle_vertices[2] = z;
    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = width_2;
    rectangle_vertices[6] = -height_2;
    rectangle_vertices[7] = z;
    //BOTTOM LEFT VERTICES
    rectangle_vertices[10] = -width_2;
    rectangle_vertices[11] = -height_2;
    rectangle_vertices[12] = z;
    //
    rectangle_vertices[15] = -width_2;
    rectangle_vertices[16] = height_2;
    rectangle_vertices[17] = z;

    rectangle->texture_id = TextureManager::getTextureId(textureFilename);
    rectangle->vbo_id = prepareVBO(rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_initRectangle(DE_Rectangle * rectangle, const char * textureFilename, GLfloat x_top_left, GLfloat y_top_left, GLfloat x_bottom_right, GLfloat y_bottom_right, GLfloat z)
{
    if(shaderInited == 0)
    {
        cout << "[ERROR] DE_initRectangle shader not inited" << endl;
        exit(-1);
    }

    //TOP RIGHT VERTICES
    rectangle_vertices[0] = x_bottom_right;
    rectangle_vertices[1] = y_top_left;
    rectangle_vertices[2] = z;
    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = x_bottom_right;
    rectangle_vertices[6] = y_bottom_right;
    rectangle_vertices[7] = z;
    //BOTTOM LEFT VERTICES
    rectangle_vertices[10] = x_top_left;
    rectangle_vertices[11] = y_bottom_right;
    rectangle_vertices[12] = z;
    //TOP LEFT VERTICES
    rectangle_vertices[15] = x_top_left;
    rectangle_vertices[16] = y_top_left;
    rectangle_vertices[17] = z;

    rectangle->texture_id = TextureManager::getTextureId(textureFilename);
    rectangle->vbo_id = prepareVBO(rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_initRectangle_1(DE_Rectangle * rectangle, GLuint textureId, GLfloat x_top_left, GLfloat y_top_left, GLfloat x_bottom_right, GLfloat y_bottom_right, GLfloat z)
{
    if(shaderInited == 0)
    {
        cout << "[ERROR] DE_initRectangle shader not inited" << endl;
        exit(-1);
    }

    //TOP RIGHT VERTICES
    rectangle_vertices[0] = x_bottom_right;
    rectangle_vertices[1] = y_top_left;
    rectangle_vertices[2] = z;
    //BOTTOM RIGHT VERTICES
    rectangle_vertices[5] = x_bottom_right;
    rectangle_vertices[6] = y_bottom_right;
    rectangle_vertices[7] = z;
    //BOTTOM LEFT VERTICES
    rectangle_vertices[10] = x_top_left;
    rectangle_vertices[11] = y_bottom_right;
    rectangle_vertices[12] = z;
    //TOP LEFT VERTICES
    rectangle_vertices[15] = x_top_left;
    rectangle_vertices[16] = y_top_left;
    rectangle_vertices[17] = z;

    rectangle->texture_id = textureId;
    rectangle->vbo_id = prepareVBO(rectangle_vertices, sizeof(rectangle_vertices));
}

void DE_drawRectangle(DE_Rectangle * rectangle){
    glUseProgram(shader_program);
    {
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(rectangle->projection));
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(rectangle->view));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(rectangle->model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rectangle->texture_id);
        glUniform1i(textureUnitLocation, GL_TEXTURE0);

        glBindBuffer(GL_ARRAY_BUFFER, rectangle->vbo_id);
        {
            glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(position_location);

            glVertexAttribPointer(texCoord_attrib_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(texCoord_attrib_location);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glUseProgram(0);
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
