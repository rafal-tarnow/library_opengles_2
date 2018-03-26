#include "TriangleStrip_Renderer.hpp"
#include "../TextureManager/texture_manager.hpp"
#include <iostream>
#include "../../system_log.hpp"

using namespace std;

static GLint compileShaders(const char *vertex_shader_source, const char *fragment_shader_source);

static const GLchar* vertex_shader_source =
        "#version 100                           \n"
        "attribute vec3 position;               \n"
        "                                       \n"
        "uniform mat4 model;                    \n"
        "uniform mat4 view;                     \n"
        "uniform mat4 projection;               \n"
        "                                       \n"
        "varying vec2 v_TexCoordinate;          \n"
        "                                       \n"
        "void main() {                          \n"
        "   gl_Position =  projection * view * model * vec4(position, 1.0);  \n"
        "   v_TexCoordinate = (position.xy)/5.0;     \n"
        "}                                      \n";


static const GLchar* fragment_shader_source =
        "#version 100                                               \n"
        "precision mediump float;                                   \n"
        "uniform vec4 colour;                                       \n"
        "                                                           \n"
        "varying vec2 v_TexCoordinate;                              \n"
        "uniform sampler2D textureUnit;                             \n"
        "                                                           \n"
        "void main() {                                              \n"
        "   //gl_FragColor = colour;                                \n"
        "   gl_FragColor = texture2D(textureUnit,v_TexCoordinate);  \n"
        "}                                                          \n";



static GLint projectionMatrixLocation;
static GLint colourLocation;
static GLint viewMatrixLocation;
static GLint modelMatrixLocation;
static GLint position_location;
static GLint textureUnitLocation;
static GLuint shader_program;
static int shaderInited = 0;


static GLuint generateVBO(){
    GLuint vbo;
    glGenBuffers(1,&vbo);
    return vbo;
}

static void updateVBOdata(GLuint vbo, const GLvoid * data, GLsizeiptr number_of_bytes)
{
    if((number_of_bytes>0) && (data!=nullptr) && (vbo!=0))
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, number_of_bytes, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
static GLuint initShader()
{
    if(shaderInited == 0)
    {
        shader_program = compileShaders(vertex_shader_source, fragment_shader_source);

        position_location = glGetAttribLocation(shader_program, "position");

        projectionMatrixLocation = glGetUniformLocation(shader_program, "projection");
        viewMatrixLocation = glGetUniformLocation(shader_program, "view");
        modelMatrixLocation = glGetUniformLocation(shader_program, "model");
        colourLocation = glGetUniformLocation(shader_program, "colour");
              textureUnitLocation = glGetUniformLocation (shader_program, "textureUnit" );

        shaderInited = 1;
    }
    return shader_program;
}

void TS_initTriangleStrip(TS_TriangleStrip * triangleStrip, float * verticlesTable, int tableSize, glm::vec4 color)
{
    if(shaderInited == 0)
    {
        initShader();
    }

    glUseProgram(shader_program);
    {
        glUniform4fv(colourLocation,1,glm::value_ptr(color));
    }
    glUseProgram(0);

    triangleStrip->numberOfVerticles = tableSize/3;
    triangleStrip->vbo_id = generateVBO();

    updateVBOdata(triangleStrip->vbo_id, verticlesTable, tableSize * sizeof(float));
}

void TS_initTriangleStrip(TS_TriangleStrip * triangleStrip, glm::vec3 * verticlesTable, int tableSize, glm::vec4 color)
{
    if(shaderInited == 0)
    {
        initShader();
    }

    glUseProgram(shader_program);
    {
        glUniform4fv(colourLocation,1,glm::value_ptr(color));
    }
    glUseProgram(0);

    triangleStrip->numberOfVerticles = tableSize;
    triangleStrip->vbo_id = generateVBO();

    updateVBOdata(triangleStrip->vbo_id, glm::value_ptr(verticlesTable[0]), tableSize*3*4);
}

void TS_initTriangleStrip(TS_TriangleStrip * triangleStrip, glm::vec3 * verticlesTable, int tableSize, GLuint textureId)
{
    if(shaderInited == 0)
    {
        initShader();
    }

    glUseProgram(shader_program);
    {
        glm::vec4 defaultColor(0.0f, 1.0f, 0.0f, 1.0f);
        glUniform4fv(colourLocation,1,glm::value_ptr(defaultColor));
    }
    glUseProgram(0);

    triangleStrip->numberOfVerticles = tableSize;
    triangleStrip->vbo_id = generateVBO();
    triangleStrip->texture_id = textureId;

    updateVBOdata(triangleStrip->vbo_id, glm::value_ptr(verticlesTable[0]), tableSize*3*4);
}


void TS_drawTriangleStrip(TS_TriangleStrip * triangleStrip)
{
    glLineWidth(10.0);
    glUseProgram(shader_program);
    {
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(triangleStrip->projection));
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(triangleStrip->view));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(triangleStrip->model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, triangleStrip->texture_id);
        glUniform1i(textureUnitLocation, GL_TEXTURE0);

        glBindBuffer(GL_ARRAY_BUFFER, triangleStrip->vbo_id);
        {
            glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, triangleStrip->numberOfVerticles);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glUseProgram(0);
}

void TS_deleteTriangleStrip(TS_TriangleStrip * lineStrip)
{
    glDeleteBuffers(1, &(lineStrip->vbo_id));
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
