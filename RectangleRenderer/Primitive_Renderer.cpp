#include "Primitive_Renderer.hpp"
#include "../TextureManager/texture_manager.hpp"
#include <iostream>
#include "../../system_log.hpp"

using namespace std;

static GLint compileShaders(const char *vertex_source, const char *fragment_source);

static const GLchar* solid_vertex_shader_source =
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


static const GLchar* solid_fragment_shader_source =
        "#version 100                                               \n"
        "precision mediump float;                                   \n"
        "uniform vec4 colour;                                       \n"
        "                                                           \n"
        "void main() {                                              \n"
        "   gl_FragColor = colour;                                  \n"
        "}                                                          \n";

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
        {
            glBufferData(GL_ARRAY_BUFFER, number_of_bytes, data, usage);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

//COLOUR PROGRAM LOCATIONS
static GLint solid_projectionMatrixLocation;
static GLint solid_viewMatrixLocation;
static GLint solid_modelMatrixLocation;
static GLint solid_position_location;
static GLint solid_colourLocation;
static GLint solid_pointSizeLocation;
static GLuint solid_shader_program;

//TEXTURE PROGRAM LOCATIONS
static GLint position_location_tex;
static GLint texCoord_attrib_location_tex;
static GLint textureUnitLocation_tex;
static GLint projectionMatrixLocation_tex;
static GLint viewMatrixLocation_tex;
static GLint modelMatrixLocation_tex;
static GLuint texture_shader_program;


static int solidColourShaderInited = 0;
static int textureShaderInited = 0;

static GLuint initSolidColourShader()
{
    assert(solidColourShaderInited == 0);
    if(solidColourShaderInited == 0)
    {
        solid_shader_program = compileShaders(solid_vertex_shader_source, solid_fragment_shader_source);
        
        solid_position_location = glGetAttribLocation(solid_shader_program, "position");
        
        solid_projectionMatrixLocation = glGetUniformLocation(solid_shader_program, "projection");
        solid_viewMatrixLocation = glGetUniformLocation(solid_shader_program, "view");
        solid_modelMatrixLocation = glGetUniformLocation(solid_shader_program, "model");
        solid_colourLocation = glGetUniformLocation(solid_shader_program, "colour");
        solid_pointSizeLocation = glGetUniformLocation(solid_shader_program, "pointSize");
        
        solidColourShaderInited = 1;
    }
    else
    {
        cout << "[WARNING] shader arlady compiled" << endl;
    }
    return solid_shader_program;
}

void initTextureShader(){
    assert(textureShaderInited == 0);
    if(textureShaderInited == 0)
    {
        texture_shader_program = compileShaders(texture_vertex_shader_source, texture_fragment_shader_source);
        
        position_location_tex = glGetAttribLocation(texture_shader_program, "position");
        texCoord_attrib_location_tex = glGetAttribLocation(texture_shader_program,"texCoord");
        textureUnitLocation_tex = glGetUniformLocation (texture_shader_program, "textureUnit" );
        
        projectionMatrixLocation_tex = glGetUniformLocation(texture_shader_program, "projection");
        viewMatrixLocation_tex = glGetUniformLocation(texture_shader_program, "view");
        modelMatrixLocation_tex = glGetUniformLocation(texture_shader_program, "model");
        
        textureShaderInited = 1;
    }
    else
    {
        cout << "[WARNING] shader arlady compiled" << endl;
    }
}



void PR_init(Primitive * primitive, float * verticlesTable, int tableSize, glm::vec4 color, GLenum mode, GLenum vbo_usage)
{
    if(solidColourShaderInited == 0)
    {
        initSolidColourShader();
    }
    
    primitive->colour = color;
    primitive->vbo_usage = vbo_usage;
    primitive->mode = mode;
    primitive->numberOfVerticles = tableSize/3;
    primitive->vbo_id = generateVBO();
    
    updateVBOdata(primitive->vbo_id, verticlesTable, tableSize * sizeof(float), primitive->vbo_usage);
}

void PR_init(Primitive * primitive, glm::vec3 * verticlesTable, int tableSize, glm::vec4 color, GLenum mode, GLenum vbo_usage )
{
    if(solidColourShaderInited == 0)
    {
        initSolidColourShader();
    }
    
    primitive->colour = color;
    primitive->vbo_usage = vbo_usage;
    primitive->mode = mode;
    primitive->numberOfVerticles = tableSize;
    primitive->vbo_id = generateVBO();
    
    updateVBOdata(primitive->vbo_id, glm::value_ptr(verticlesTable[0]), tableSize*3*4, primitive->vbo_usage);
}

void PR_setMode(Primitive * primitive, GLenum mode)
{
    primitive->mode = mode;
}

void PR_setPointSize(Primitive * primitive, float pointSize)
{
    glUseProgram(solid_shader_program);
    {
        glUniform1f(solid_pointSizeLocation, pointSize);
    }
    glUseProgram(0);
}

void PR_setColour(Primitive * primitive, glm::vec4 colour)
{
    primitive->colour = colour;
}

void PR_setTexture(Primitive * primitive, GLuint texture)
{
    if(textureShaderInited == 0)
    {
        initTextureShader();
    }
    primitive->texture_id = texture;
}

void PR_setVerticles(Primitive * primitive, glm::vec3 * verticlesTable, int tableSize)
{
    primitive->numberOfVerticles = tableSize;
    updateVBOdata(primitive->vbo_id, glm::value_ptr(verticlesTable[0]), tableSize*3*4, primitive->vbo_usage);
}


void PR_draw(Primitive * primitive, GLfloat width)
{
    if(primitive->texture_id != 0)
    {
        
    }
    else
    {
        glLineWidth(width);
        glUseProgram(solid_shader_program);
        {
            glUniform4fv(solid_colourLocation,1,glm::value_ptr(primitive->colour));
            glUniformMatrix4fv(solid_projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(primitive->projection));
            glUniformMatrix4fv(solid_viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(primitive->view));
            glUniformMatrix4fv(solid_modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(primitive->model));
            
            glBindBuffer(GL_ARRAY_BUFFER, primitive->vbo_id);
            {
                glEnableVertexAttribArray(solid_position_location);
                glVertexAttribPointer(solid_position_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
                
                
                glDrawArrays(primitive->mode, 0, primitive->numberOfVerticles);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        glUseProgram(0);
    }
}

void PR_delete(Primitive * primitive)
{
    glDeleteBuffers(1, &(primitive->vbo_id));
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
