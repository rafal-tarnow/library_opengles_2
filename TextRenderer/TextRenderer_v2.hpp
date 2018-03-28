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


class Atlas{
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
    void loadCommon(FT_Library &ft, FT_Face &face, GLuint &fontSize);
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
        unsigned int glyph_bitmap_rows;
        unsigned int glyph_bitmap_width;
        unsigned char*  glyph_bitmap_buffer = nullptr;
        FT_Int glyph_bitmap_left;
        FT_Int glyph_bitmap_top;
        FT_Pos glyph_advance_x;
    }CharacterData;
    std::map<char, CharacterData> charactersMap;


    Atlas atlas;

    struct {
        GLuint textBufferTexture;
        char * textBuffer;
    }TextData;

    GLint textColourLocation;
    GLint projectionMatrixLocation;
    CharacterData charData_tmp;
    std::string::const_iterator c;

    GLfloat current_viewport_width_in_pixels;
    GLfloat current_viewport_height_in_pixels;

    GLuint EBO;

    std::string previous_string;
};

