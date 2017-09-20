#include "TextRenderer_v1.hpp"

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

static GLuint shader_program;

static GLuint prepareVBO(const GLfloat * data, GLsizeiptr size);
GLint compileShaders(const char *vertex_shader_source, const char *fragment_shader_source);
void drawGlyphToConsole(FT_Face &face);

static const GLchar* vertex_shader_source =
        "#version 100                               \n"
        "attribute vec3 position;                   \n"
        "attribute vec2 texCoord;                   \n"
        "varying vec2 v_TexCoordinate;              \n"
        "                                           \n"
        "//uniform mat4 model;                      \n"
        "//uniform mat4 view;                       \n"
        "uniform mat4 projection;                   \n"
        "                                           \n"
        "void main() {                              \n"
        "   gl_Position = projection * /*view * model * */vec4(position, 1.0);  \n"
        "   v_TexCoordinate = texCoord;             \n"
        "}                                          \n";


static const GLchar* fragment_shader_source =
        "#version 100                               \n"
        "                                           \n"
        "precision mediump float;                   \n"
        "                                           \n"
        "varying vec2 v_TexCoordinate;              \n"
        "uniform sampler2D textureUnit;             \n"
        "                                           \n"
        "void main() {                              \n"
        "   vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(textureUnit,v_TexCoordinate).a);   \n"
        "   gl_FragColor = /*vec4(0.5,0.5,0.5,0.3) +*/ vec4(0.0,0.0,0.0,1.0) * sampled ;     \n"
        "}                                                      \n";



TextRenderer_v1::TextRenderer_v1(int viewport_width_in_pixels, int viewport_height_in_pixels){

    shader_program = compileShaders(vertex_shader_source, fragment_shader_source);

    position_location = glGetAttribLocation(shader_program, "position");
    texCoord_attrib_location = glGetAttribLocation(shader_program,"texCoord");
    textureUnitLocation = glGetUniformLocation (shader_program, "textureUnit" );



    projectionMatrixLocation = glGetUniformLocation(shader_program, "projection");
    glUseProgram(shader_program);
    {
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::ortho(static_cast<GLfloat>(-viewport_width_in_pixels/2), static_cast<GLfloat>(viewport_width_in_pixels/2), static_cast<GLfloat>(-viewport_height_in_pixels/2), static_cast<GLfloat>(viewport_height_in_pixels/2))));
    }
    glUseProgram(0);

    vbo = prepareVBO(verticles_table, sizeof(verticles_table));
}

TextRenderer_v1::~TextRenderer_v1(){
    glDeleteBuffers(1, &vbo);
}

void TextRenderer_v1::RenderText(std::string text){


//    GLfloat verticles_table[20]= {
//        107.0f, 99.0f, 0.0f,    1.0f,1.0f,
//        107.0f, 0.0f, 0.0f,    1.0f,0.0f,
//        0.0f, 0.0f, 0.0f,   0.0f,0.0f,
//        0.0f, 99.0f, 0.0f,    0.0f,1.0f
//    };


    charData_tmp = charactersMap.at(*text.begin());

    GLfloat pen_x = 0.0f;
    GLfloat pen_y = 0.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(shader_program);
    {

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        //glActiveTexture(GL_TEXTURE0);
        //glUniform1i(textureUnitLocation, GL_TEXTURE0);

        charData_tmp = charactersMap.at('A');

        glBindTexture(GL_TEXTURE_2D, charData_tmp.characterTextureID);
        verticles_table[0] = pen_x + charData_tmp.glyph_bitmap_width;
        verticles_table[5] = pen_x + charData_tmp.glyph_bitmap_width;
        verticles_table[10] = pen_x;
        verticles_table[15] = pen_x;
        verticles_table[1] = charData_tmp.glyph_bitmap_rows;
        verticles_table[16] = charData_tmp.glyph_bitmap_rows;
        glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(verticles_table), verticles_table);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


        pen_x = 25.0f;
        pen_y = 0.0f;
        charData_tmp = charactersMap.at('B');

        glBindTexture(GL_TEXTURE_2D, charData_tmp.characterTextureID);
        verticles_table[0] = pen_x + charData_tmp.glyph_bitmap_width;
        verticles_table[5] = pen_x + charData_tmp.glyph_bitmap_width;
        verticles_table[10] = pen_x;
        verticles_table[15] = pen_x;
        verticles_table[1] = charData_tmp.glyph_bitmap_rows;
        verticles_table[16] = charData_tmp.glyph_bitmap_rows;
        glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(verticles_table), verticles_table);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        pen_x = 50.0f;
        pen_y = 0.0f;
        charData_tmp = charactersMap.at('j');

        glBindTexture(GL_TEXTURE_2D, charData_tmp.characterTextureID);
        verticles_table[0] = pen_x + charData_tmp.glyph_bitmap_width;
        verticles_table[5] = pen_x + charData_tmp.glyph_bitmap_width;
        verticles_table[10] = pen_x;
        verticles_table[15] = pen_x;
        verticles_table[1] = charData_tmp.glyph_bitmap_rows;
        verticles_table[16] = charData_tmp.glyph_bitmap_rows;
        glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(verticles_table), verticles_table);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture (GL_TEXTURE_2D, 0);

    }
    glUseProgram(0);
}

void TextRenderer_v1::Load(std::string font, GLuint fontSize){


    FT_Library ft;

    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, /*font.c_str()*/ "/usr/share/fonts/truetype/liberation/LiberationSerif-Regular.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    if(FT_Select_Charmap(face, FT_ENCODING_UNICODE ))
        std::cout << "ERROR Select Charmap" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint TextureID;

    for(char c = '0'; c <= 'z'; c++)
    {

        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        glGenTextures(1, &TextureID);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture (GL_TEXTURE_2D, 0);

        // Destroy FreeType once we're finished

        cout << "ZNAK: " << c << endl;
        cout << "face->glyph->advance.x = " << face->glyph->advance.x << endl;
        cout << "face->glyph->advance.x in pixels = " << face->glyph->advance.x/64 << endl;
        cout << "face->glyph->bitmap.width = " << face->glyph->bitmap.width << endl;
        cout << "face->glyph->bitmap.rows = " << face->glyph->bitmap.rows << endl;

        charData_tmp.characterTextureID = TextureID;
        charData_tmp.glyph_bitmap_width = face->glyph->bitmap.width;
        charData_tmp.glyph_bitmap_rows = face->glyph->bitmap.rows;

        charactersMap[c] = charData_tmp;
    }

    FT_Done_Face (face);
    FT_Done_FreeType (ft);
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
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << endl <<  infoLog << endl;
    }else{
        cout << "SUCCESSFUL::SHADER::VERTEX::COMPILATION" << endl;
    }

    /* Fragment shader */
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, INFOLOG_LEN, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }else{
        cout << "SUCCESSFUL::SHADER::FRAGMENT::COMPILATION" << endl;
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
    }else{
        cout << "SUCCESSFUL::SHADER::PROGRAM::LINKING" << endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_program;
}

GLuint TextRenderer_v1::prepareVBO(const GLfloat * data, GLsizeiptr size){
    GLuint vbo;

    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(position_location);

    glVertexAttribPointer(texCoord_attrib_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(texCoord_attrib_location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

void drawGlyphToConsole(FT_Face &face){

    for(unsigned int i = 0; i < face->glyph->bitmap.rows; i++){
        for(unsigned int j = 0; j < face->glyph->bitmap.width; j++){

            int alpha_value =   (int)(face->glyph->bitmap.buffer[i*face->glyph->bitmap.width + j]) / 26;

            if(alpha_value > 0){
                cout << alpha_value;
            }else{
                cout << " ";
            }


        }
        cout << endl;
    }
}

