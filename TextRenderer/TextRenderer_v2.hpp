#pragma once

#include <GLES2/gl2.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <library_opengles_2/Debug/Debug.hpp>

const int MAX_STRING_LENGHT = 5000;

#warning "TextRenderer_v2 dorobienie czyszczenia statycznych atlasow"
#warning "TextRenderer_v2 dorobienie kwartatowej tekstury open gl zamiast prostokatnej"
#warning "TextRenderer_v2 dorobienie sprawdzania maksymalnej tekstury dla atlasu"

using namespace std;


class Atlas_gl{
public:

    typedef struct{
        unsigned int glyph_bitmap_width; //converted
        FT_Int glyph_bitmap_left;   //converted
        FT_Int glyph_bitmap_top;    //converted
        unsigned int glyph_bitmap_rows; //converted
        FT_Pos glyph_advance_x;

        double u_coord_right;
        double u_coord_left;
        double v_coord_top;
        double v_coord_bottom;
    }GlyphData;

    unsigned int atlas_width;
    unsigned int atlas_rows;
    GLuint rectangle_AtlasTextureId;
    GLuint square_AtlasTextureId;

    std::map<char, GlyphData> glyph_map;
private:
    DBG_COUNT("Atlas_gl");
};

class TextRenderer_v2
{
public:
    typedef enum
    {
        TEXT_LEFT,
        TEXT_CENTER,
        TEXT_RIGHT
    }TextPosition;

public:
    TextRenderer_v2(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels,  glm::vec4 textColor = glm::vec4(1.0,0.0,0.0,1.0));
    ~TextRenderer_v2();

    void onVievportResize(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels);

    void Load(std::string fontName, std::string fontFilePath, GLuint fontSize);
    void LoadFromMemory(std::string fontName, const unsigned char * fontFileData, int data_size, GLuint fontSize);


    void setColour(glm::vec4 colour);
    void setCustomPV(glm::mat4 P, glm::mat4 V);
    void RenderText(std::string text, GLfloat x_pixel, GLfloat y_pixel, TextPosition origin = TEXT_RIGHT);
    void RenderText(uint8_t instance, std::string text,  GLfloat x, GLfloat y, TextPosition origin = TEXT_RIGHT);
    void RenderText(uint8_t instance, std::string text, glm::mat4 model, TextPosition origin = TEXT_RIGHT);

private:
    void prepareOpenGLSquareAtlas(FT_Library &ft, FT_Face &face, GLuint &fontSize, Atlas_gl &atlas_gl);
    //void prepareOpenGLRectangleAtlas(FT_Library &ft, FT_Face &face, GLuint &fontSize, Atlas_gl &atlas_gl);

    bool doOptymalization_2(GLfloat x_right);
    bool doOptymalization_1(GLfloat  x_left, GLfloat y_top, GLfloat y_bottom );
    GLuint prepareVBO(const GLfloat * data, GLsizeiptr size);

    void debug_RenderRectangleAtlas(GLfloat x, GLfloat y);
    void debug_RenderSquareAtlas(GLuint vbo, GLfloat x, GLfloat y);

    Atlas_gl * current_atlas;

    glm::vec4 viewport = glm::vec4(0,0,0,0);


    class TextInstance{
    public:
        TextInstance()
        {
            previous_string = "";
            textLenght = 0.0f;
            vbo = 0;
        }
        string previous_string;
        GLfloat textLenght;
        GLuint vbo;
    private:
       DBG_COUNT("TextInstance");
    };

    //GOODS
    GLuint EBO;
    map<uint8_t,TextInstance> vboBuffersMap;

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

    bool useCustomPV = false;
    glm::mat4 mProjection = glm::mat4(1);
    glm::mat4 mView = glm::mat4(1);

    //STATICS
    static GLuint shader_program;
    static GLint position_location;
    static GLint texCoord_attrib_location;
    static GLint textureMapLocation;
    static GLint textColourLocation;
    static GLint projectionMatrixLocation;
    static GLint viewMatrixLocation;
    static GLint modelMatrixLocation;

    static map<string, map<GLuint , Atlas_gl *>> mapaAtlasow;

  DBG_COUNT("TextRenderer_v2");

};

