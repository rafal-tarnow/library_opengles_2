#pragma once

#include "opengl_includes.hpp"

#include <string>
#include <map>

const int MAX_STRING_LENGHT = 5000;

class TextRenderer_v2
{
public:
    TextRenderer_v2(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels);
    ~TextRenderer_v2();

    void onVievportResize(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels);

    void Load(std::string font, GLuint fontSize);
    void RenderText(std::string text, GLfloat x_pixel, GLfloat y_pixel);

private:
    void prepareGlypAtlas(unsigned int total_width, unsigned int max_rows);
    bool doOptymalization_2(GLfloat x_right);
    bool doOptymalization_1(GLfloat  x_left, GLfloat y_top, GLfloat y_bottom );
    GLuint prepareVBO(const GLfloat * data, GLsizeiptr size);
    GLfloat verticles_table[20*MAX_STRING_LENGHT]= {
        107.0f, 99.0f, 0.0f,    1.0f,0.0f,
        107.0f, 0.0f, 0.0f,    1.0f,1.0f,
        0.0f, 0.0f, 0.0f,   0.0f,1.0f,
        0.0f, 99.0f, 0.0f,    0.0f,0.0f
    };

    GLuint indices[6*MAX_STRING_LENGHT] = {
        0, 1, 2,    1, 2, 3,
        4, 5, 6,    5, 6, 7
    };

    GLuint vbo;
    GLint position_location;
    GLint texCoord_attrib_location;
    GLint textureUnitLocation;



    typedef struct{
        GLuint characterTextureID;
        GLfloat glyph_bitmap_rows;
        GLfloat glyph_bitmap_width;
        unsigned char*  glyph_bitmap_buffer = nullptr;
        GLfloat glyph_bitmap_left;
        GLfloat glyph_bitmap_top;
        GLfloat glyph_advance_x;
        GLfloat u_coord_right;
        GLfloat u_coord_left;
        GLfloat v_coord_top;
        GLfloat v_coord_bottom;
    }CharacterData;

    std::map<char, CharacterData> charactersMap;


    struct {
        unsigned int atlas_width;
        unsigned int atlas_rows;
        GLuint glyphAtlasTextureId;
    } GlyphAtlasData;

    struct {
        GLuint textBufferTexture;
        char * textBuffer;
    }TextData;

    GLint projectionMatrixLocation;
    CharacterData charData_tmp;
    std::string::const_iterator c;

    GLfloat current_viewport_width_in_pixels;
    GLfloat current_viewport_height_in_pixels;

    GLuint EBO;

    std::string previous_string;
};

