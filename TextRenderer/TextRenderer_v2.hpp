#pragma once

#include <GLES2/gl2.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int MAX_STRING_LENGHT = 5000;

#warning "Przerobic TextRenderer_v2 na singletona"


class Atlas_gl{
public:
    typedef struct{
        unsigned int glyph_bitmap_width; //converted
        FT_Int glyph_bitmap_left;   //converted
        FT_Int glyph_bitmap_top;    //converted
        unsigned int glyph_bitmap_rows; //converted
        FT_Pos glyph_advance_x;

        GLfloat u_coord_right;
        GLfloat u_coord_left;
        GLfloat v_coord_top;
        GLfloat v_coord_bottom;
    }GlyphData;

    unsigned int atlas_width;
    unsigned int atlas_rows;
    GLuint glyphAtlasTextureId;

    std::map<char, GlyphData> glyph_map;
};

class Atlas_ft{
public:
    typedef struct{
        unsigned int glyph_bitmap_rows;
        unsigned int glyph_bitmap_width;
        unsigned char*  glyph_bitmap_buffer = nullptr;
        FT_Int glyph_bitmap_left;
        FT_Int glyph_bitmap_top;
        FT_Pos glyph_advance_x;
    }CharacterData;


    unsigned int total_width;
    unsigned int max_rows;
    std::map<char, CharacterData> charactersMap;
};

class TextRenderer_v2
{
public:
    TextRenderer_v2(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels,  glm::vec4 textColor = glm::vec4(1.0,0.0,0.0,1.0));
    ~TextRenderer_v2();

    void onVievportResize(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels);

    void Load(std::string fontFilePath, GLuint fontSize);
    void LoadFromMemory(const unsigned char * fontFileData, int data_size, GLuint fontSize);

    void RenderText(std::string text, GLfloat x_pixel, GLfloat y_pixel);

private:
    void prepareOpenGLAtlas(FT_Library &ft, FT_Face &face, GLuint &fontSize, Atlas_gl &atlas_gl);
    bool doOptymalization_2(GLfloat x_right);
    bool doOptymalization_1(GLfloat  x_left, GLfloat y_top, GLfloat y_bottom );
    GLuint prepareVBO(const GLfloat * data, GLsizeiptr size);





    Atlas_ft atlas_ft;
    Atlas_gl atlas_gl;



    glm::vec4 viewport = glm::vec4(0,0,0,0);
    std::string previous_string;


    //GOODS
    GLuint EBO;
    GLuint vbo;

    GLfloat verticles_table[20*MAX_STRING_LENGHT]= {
        0.0f, 0.0f, 0.0f,   1.0f,0.0f,
        0.0f, 0.0f, 0.0f,   1.0f,1.0f,
        0.0f, 0.0f, 0.0f,   0.0f,1.0f,
        0.0f, 0.0f, 0.0f,   0.0f,0.0f
    };

    GLuint indices[6*MAX_STRING_LENGHT] = {
        0, 1, 2,    1, 2, 3,
        4, 5, 6,    5, 6, 7
    };

    glm::vec4 mTextColour;
    glm::mat4 mProjection;

    //STATICS
    static GLuint shader_program;
    static GLint position_location;
    static GLint texCoord_attrib_location;
    static GLint textureUnitLocation;
    static GLint textColourLocation;
    static GLint projectionMatrixLocation;


};

