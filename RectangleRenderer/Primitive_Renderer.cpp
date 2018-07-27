#include "Primitive_Renderer.hpp"
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
        "uniform float pointSize;"
        "                                       \n"
        "void main() {                          \n"
        "   gl_Position =  projection * view * model * vec4(position, 1.0);  \n"
        "   gl_PointSize = pointSize;                \n"
        "}                                      \n";


static const GLchar* fragment_shader_source =
        "#version 100                                               \n"
        "precision mediump float;                                   \n"
        "uniform vec4 colour;                                       \n"
        "                                                           \n"
        "void main() {                                              \n"
        "   gl_FragColor = colour;                                  \n"
        "}                                                          \n";


static GLuint generateVBO(){
    GLuint vbo;
    glGenBuffers(1,&vbo);
    return vbo;
}

static void updateVBOdata(GLuint vbo, const GLvoid * data, GLsizeiptr number_of_bytes, GLenum usage)
{
    if((number_of_bytes>0) && (data!=nullptr) && (vbo!=0))
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, number_of_bytes, data, usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

static GLint projectionMatrixLocation;
static GLint viewMatrixLocation;
static GLint modelMatrixLocation;
static GLint position_location;
static GLint colourLocation;
static GLint pointSizeLocation;
static GLuint shader_program;
static int shaderInited = 0;

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
        pointSizeLocation = glGetUniformLocation(shader_program, "pointSize");

        shaderInited = 1;
    }
    else
    {
        cout << "[WARNING] shader arlady compiled" << endl;
    }
    return shader_program;
}

void PR_init(PR_LineStrip * lineStrip, float * verticlesTable, int tableSize, glm::vec4 color, GLenum mode, GLenum vbo_usage)
{
    if(shaderInited == 0)
    {
        initShader();
    }

    lineStrip->colour = color;
    lineStrip->vbo_usage = vbo_usage;
    lineStrip->mode = mode;
    lineStrip->numberOfVerticles = tableSize/3;
    lineStrip->vbo_id = generateVBO();

    updateVBOdata(lineStrip->vbo_id, verticlesTable, tableSize * sizeof(float), lineStrip->vbo_usage);
}

void PR_init(PR_LineStrip * lineStrip, glm::vec3 * verticlesTable, int tableSize, glm::vec4 color, GLenum mode, GLenum vbo_usage )
{
    if(shaderInited == 0)
    {
        initShader();
    }

    lineStrip->colour = color;
    lineStrip->vbo_usage = vbo_usage;
    lineStrip->mode = mode;
    lineStrip->numberOfVerticles = tableSize;
    lineStrip->vbo_id = generateVBO();

    updateVBOdata(lineStrip->vbo_id, glm::value_ptr(verticlesTable[0]), tableSize*3*4, lineStrip->vbo_usage);
}

void PR_setMode(PR_LineStrip * lineStrip, GLenum mode)
{
    lineStrip->mode = mode;
}

void PR_setPointSize(PR_LineStrip * lineStrip, float pointSize)
{
    glUseProgram(shader_program);
    {
        glUniform1f(pointSizeLocation, pointSize);
    }
    glUseProgram(0);
}

void PR_setColour(PR_LineStrip * lineStrip, glm::vec4 colour)
{
    lineStrip->colour = colour;
}

void PR_updateData(PR_LineStrip * lineStrip, glm::vec3 * verticlesTable, int tableSize)
{
    lineStrip->numberOfVerticles = tableSize;
    updateVBOdata(lineStrip->vbo_id, glm::value_ptr(verticlesTable[0]), tableSize*3*4, lineStrip->vbo_usage);
}


void PR_draw(PR_LineStrip * lineStrip, GLfloat width)
{
    glLineWidth(width);
    glUseProgram(shader_program);
    {
        glUniform4fv(colourLocation,1,glm::value_ptr(lineStrip->colour));
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(lineStrip->projection));
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(lineStrip->view));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(lineStrip->model));

        glBindBuffer(GL_ARRAY_BUFFER, lineStrip->vbo_id);
        {
            glEnableVertexAttribArray(position_location);
            glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);


            glDrawArrays(lineStrip->mode, 0, lineStrip->numberOfVerticles);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glUseProgram(0);
}

void PR_delete(PR_LineStrip * lineStrip)
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
