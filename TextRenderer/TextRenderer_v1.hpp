#pragma once

#include "opengl_includes.hpp"

#include <string>
#include <map>

class TextRenderer_v1
{
public:
    TextRenderer_v1(int viewport_width_in_pixels, int viewport_height_in_pixels);
    ~TextRenderer_v1();

    void Load(std::string font, GLuint fontSize);
    void RenderText(std::string text);

private:
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
         unsigned int glyph_bitmap_rows;
         unsigned int glyph_bitmap_width;
    }CharacterData;

    std::map<char, CharacterData> charactersMap;

    GLint projectionMatrixLocation;
     CharacterData charData_tmp;
};

