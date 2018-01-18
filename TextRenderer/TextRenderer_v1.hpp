#pragma once

#include "opengl_includes.hpp"

#include <string>
#include <map>

class TextRenderer_v1
{
public:
    TextRenderer_v1(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels);
    ~TextRenderer_v1();

    void onVievportResize(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels);

    void Load(std::string font, GLuint fontSize);
    void RenderText(std::string text, GLfloat x_pixel, GLfloat y_pixel);

private:
    bool doOptymalization_2(GLfloat x_right);
    bool doOptymalization_1(GLfloat  x_left, GLfloat y_top, GLfloat y_bottom );
    GLuint prepareVBO(const GLfloat * data, GLsizeiptr size);
    GLfloat verticles_table[20]= {
        107.0f, 99.0f, 0.0f,    1.0f,0.0f,
        107.0f, 0.0f, 0.0f,    1.0f,1.0f,
        0.0f, 0.0f, 0.0f,   0.0f,1.0f,
        0.0f, 99.0f, 0.0f,    0.0f,0.0f
    };
    GLuint vbo;
    GLint position_location;
    GLint texCoord_attrib_location;
    GLint textureUnitLocation;



    typedef struct{
        GLuint characterTextureID;
        GLfloat glyph_bitmap_rows;
        GLfloat glyph_bitmap_width;
        GLfloat glyph_bitmap_left;
        GLfloat glyph_bitmap_top;
        GLfloat glyph_advance_x;
    }CharacterData;

    std::map<char, CharacterData> charactersMap;

    GLint projectionMatrixLocation;
    CharacterData charData_tmp;
    std::string::const_iterator c;

    GLfloat current_viewport_width_in_pixels;
    GLfloat current_viewport_height_in_pixels;
    GLuint shader_program;
};

