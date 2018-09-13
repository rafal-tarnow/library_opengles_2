#include "TextRenderer_v2.hpp"

#include <iostream>
#include <cmath>

#include <system_log.hpp>

#include "stb_rect_pack.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;


static const GLchar* vertex_shader_source =
        "#version 100                               \n"
        "//TextRendered_v2 vertex shader            \n"
        "attribute vec3 position;                   \n"
        "attribute vec2 texCoord;                   \n"
        "varying vec2 v_TexCoordinate;              \n"
        "                                           \n"
        "uniform mat4 model;                      \n"
        "uniform mat4 view;                       \n"
        "uniform mat4 projection;                   \n"
        "                                           \n"
        "void main() {                              \n"
        "   gl_Position = projection * view * model * vec4(position, 1.0);  \n"
        "   v_TexCoordinate = texCoord;             \n"
        "}                                          \n";


static const GLchar* fragment_shader_source =
        "#version 100                               \n"
        "//TextRenderer_v2 fragmet shader           \n"
        "precision highp float;                     \n"
        "                                           \n"
        "varying vec2 v_TexCoordinate;              \n"
        "uniform sampler2D textureMap;             \n"
        "uniform vec4 textColor;                    \n"
        "void main() {                              \n"
        "       vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(textureMap,v_TexCoordinate).a);                \n"
        "       gl_FragColor = textColor * sampled;                                                         \n"
        "}                                                                                                  \n";

GLuint TextRenderer_v2::shader_program = 0;
GLint TextRenderer_v2::position_location;
GLint TextRenderer_v2::texCoord_attrib_location;
GLint TextRenderer_v2::textureMapLocation;
GLint TextRenderer_v2::textColourLocation;
GLint TextRenderer_v2::projectionMatrixLocation;
GLint TextRenderer_v2::viewMatrixLocation;
GLint TextRenderer_v2::modelMatrixLocation;
map<string, map<GLuint , Atlas_gl *>> TextRenderer_v2::mapaAtlasow;


static GLint compileShaders(const char *vertex_shader_source, const char *fragment_shader_source);
static void drawGlyphToConsole(FT_Face &face);



TextRenderer_v2::TextRenderer_v2(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels, glm::vec4 txtColor) : ObjectCounter("TextRenderer_v2")
{
    mTextColour = txtColor;

    viewport.z = viewport_width_in_pixels;
    viewport.w = viewport_height_in_pixels;

    mProjection = glm::ortho(static_cast<GLfloat>(0), static_cast<GLfloat>(viewport.z), static_cast<GLfloat>(0), static_cast<GLfloat>(viewport.w));

    if(shader_program == 0)
    {
        shader_program = compileShaders(vertex_shader_source, fragment_shader_source);

        position_location = glGetAttribLocation(shader_program, "position");
        texCoord_attrib_location = glGetAttribLocation(shader_program,"texCoord");
        textureMapLocation = glGetUniformLocation (shader_program, "textureMap" );
        textColourLocation = glGetUniformLocation(shader_program, "textColor");
        projectionMatrixLocation = glGetUniformLocation(shader_program, "projection");
        viewMatrixLocation = glGetUniformLocation(shader_program, "view");
        modelMatrixLocation = glGetUniformLocation(shader_program, "model");
    }


    //preparing EBO
    for(unsigned int i = 0; i < MAX_STRING_LENGHT; i++){
        indices[0 + i*6] = 0 + i*4;
        indices[1 + i*6] = 1 + i*4;
        indices[2 + i*6] = 2 + i*4;
        indices[3 + i*6] = 1 + i*4;
        indices[4 + i*6] = 2 + i*4;
        indices[5 + i*6] = 3 + i*4;
    }

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TextRenderer_v2::~TextRenderer_v2()
{
    glDeleteBuffers(1, &EBO);

    for(map<uint8_t, TextInstance>::iterator  pos = vboBuffersMap.begin(); pos != vboBuffersMap.end(); ++pos)
    {
        glDeleteBuffers(1, &(pos->second.vbo));
    }
}

void TextRenderer_v2::onVievportResize(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels)
{
    viewport.z = viewport_width_in_pixels;
    viewport.w = viewport_height_in_pixels;

    if(useCustomPV == false)
    {
        mProjection = glm::ortho(static_cast<GLfloat>(0), static_cast<GLfloat>(viewport.z), static_cast<GLfloat>(0), static_cast<GLfloat>(viewport.w));
    }
}

void TextRenderer_v2::Load(std::string fontName, std::string fontFilePath, GLuint fontSize)
{
    //jeżeli atlas juz istnieje
    if(mapaAtlasow.count(fontName) == 1 )
    {
        if(mapaAtlasow.at(fontName).count(fontSize) == 1)
        {
            current_atlas = mapaAtlasow.at(fontName).at(fontSize);
            return;
        }
    }

    //w przeciwnym wypadku utworz nowy atlas
    FT_Library ft;

    if (FT_Init_FreeType(&ft))
    {
        std::cout << "[ERROR]::FREETYPE: Could not init FreeType Library" << std::endl;
        exit(-1);
    }

    FT_Face face;
    if (FT_New_Face(ft, fontFilePath.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        exit(EXIT_FAILURE);
    }

    Atlas_gl * atlas_gl = new Atlas_gl();

    //prepareOpenGLRectangleAtlas(ft, face, fontSize, *atlas_gl);
    prepareOpenGLSquareAtlas(ft, face, fontSize, *atlas_gl);

    FT_Done_Face (face);
    FT_Done_FreeType (ft);

    mapaAtlasow[fontName][fontSize] = atlas_gl;
    current_atlas = atlas_gl;

}

void TextRenderer_v2::LoadFromMemory(std::string fontName, const unsigned char * font_data, int data_size, GLuint fontSize)
{
    //jeżeli atlas juz istnieje
    if(mapaAtlasow.count(fontName) == 1)
    {
        if(mapaAtlasow.at(fontName).count(fontSize) == 1)
        {
            current_atlas = mapaAtlasow.at(fontName).at(fontSize);
            return;
        }
    }

    //w przeciwnym wypadku utworz nowy atlas
    FT_Library ft;

    if (FT_Init_FreeType(&ft))
    {
        std::cout << "[ERROR]::FREETYPE: Could not init FreeType Library" << std::endl;
        exit(-1);
    }

    FT_Face face;
    if (FT_New_Memory_Face(ft, (FT_Byte*)font_data, data_size, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        exit(EXIT_FAILURE);
    }

    Atlas_gl * atlas_gl = new Atlas_gl();

    //prepareOpenGLRectangleAtlas(ft, face, fontSize, *atlas_gl);
    prepareOpenGLSquareAtlas(ft, face, fontSize, *atlas_gl);

    FT_Done_Face (face);
    FT_Done_FreeType (ft);

    mapaAtlasow[fontName][fontSize] = atlas_gl;
    current_atlas = atlas_gl;
}

//Opengl texture bytes order

//image size width 7, height 5

//      -> image height
//      |
//      |
//      3|  21  22  23  24  25  26  27
//      2|  14  15  16  17  18  19  20
//      1|  7   8   9   10  11  12  13
//      0|  0   1   2   3   4   5   6    -> bytes order in texture array
//        -----------------------------
//          0   1   2   3   4   5   6 -> image width


inline unsigned int index_bufora(int column, int row, int texture_width)
{
    return row*texture_width + column;
}


stbrp_context context;
struct stbrp_rect * rects;

void TextRenderer_v2::prepareOpenGLSquareAtlas(FT_Library &ft, FT_Face &face, GLuint &fontSize, Atlas_gl &atlas_gl){

    if(FT_Select_Charmap(face, FT_ENCODING_UNICODE ))
        std::cout << "ERROR Select Charmap" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //CALCULATE TOTAL WIDTH AND MAX HEIGHT TO KNOW HOW MUCH BUFFER ALOCATE
    unsigned int max_rows = 0;
    int total_width = 0;

    for(char c = ' '; c <= 'z'; c++)
    {
        //LOAD ONE CHAR DATA
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        //UPDATE MAX GLYPH WIDTH and MAX GLYPH HEIGHT
        if(face->glyph->bitmap.rows > max_rows){
            max_rows = face->glyph->bitmap.rows;
        }
        total_width += face->glyph->bitmap.width;

        atlas_gl.glyph_map[c].glyph_bitmap_width = face->glyph->bitmap.width;
        atlas_gl.glyph_map[c].glyph_bitmap_left = face->glyph->bitmap_left;
        atlas_gl.glyph_map[c].glyph_bitmap_top = face->glyph->bitmap_top;
        atlas_gl.glyph_map[c].glyph_bitmap_rows = face->glyph->bitmap.rows;
        atlas_gl.glyph_map[c].glyph_advance_x = face->glyph->advance.x;
    }

    //CALCULATE TOTAL SIZE FOR TO KNOW HOW BIG TEXTURE MUST BE
    uint64_t total_size = 0.0;
    for(char c = ' '; c <= 'z'; c++)
    {
        total_size = total_size + atlas_gl.glyph_map[c].glyph_bitmap_width * atlas_gl.glyph_map[c].glyph_bitmap_rows;
    }
    cout << "total size = " << total_size;


    //CALCULATE MINIMAL TEXTURE SIZE WHICH IS POSSIBLE TO LOAD GLYPHS
    uint64_t minimal_texture_size = 0;
    for(int i = 0; i < 64 ; i++)
    {
        double potega = pow(2,i);
        if(potega*potega >= total_size)
        {
            minimal_texture_size = potega;
            break;
        }
    }
    cout << "minimal_texture_size = " << minimal_texture_size << endl;
    cout.flush();


    //INIT STB RECTS
    rects = (stbrp_rect*)malloc(sizeof(stbrp_rect)*atlas_gl.glyph_map.size());

    for(char c = ' '; c <= 'z'; c++)
    {
        rects[c - ' '].id = c;
        rects[c - ' '].w = atlas_gl.glyph_map[c].glyph_bitmap_width;
        rects[c - ' '].h = atlas_gl.glyph_map[c].glyph_bitmap_rows;//
        rects[c - ' '].x = 0;
        rects[c - ' '].y = 0;
        rects[c - ' '].was_packed = 0;
    }

    int TARGET_DIM = minimal_texture_size;
    int ret;
    //TRY PACK RECTANGLES INTO SQUARE UNTIL SUCESSFULL
    do{
        struct stbrp_node * ptr_nodes = (struct stbrp_node *)malloc(sizeof(stbrp_node)*TARGET_DIM*2);

        stbrp_init_target(&context, TARGET_DIM, TARGET_DIM, ptr_nodes, TARGET_DIM*2);
        ret = stbrp_pack_rects(&context, rects, atlas_gl.glyph_map.size());

        free(ptr_nodes);
        if(ret != 1)
            TARGET_DIM = TARGET_DIM*2;

    }while(ret != 1);

    //   for (int i=0; i< atlas_gl.glyph_map.size(); i++)
    //   {
    //           printf("rect '%c' %i (pos.x = %hu, pos.y = %hu) (height = %hu, width = %hu) was_packed=%i\n", rects[i].id, rects[i].id, rects[i].x, rects[i].y, rects[i].h, rects[i].w, rects[i].was_packed);
    //   }


    //ALLOCATE DATA FOR TEXTURE BUFFER
    unsigned char *  atlas_tmp_buffer = new unsigned char[(int)((TARGET_DIM)*(TARGET_DIM))];
    unsigned int buff_size = TARGET_DIM*TARGET_DIM;

    for(unsigned int i = 0; i < buff_size; i++)
    {
        atlas_tmp_buffer[i] = 255;
    }


    int pen = 0;
    for(char c = ' '; c <= 'z'; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        unsigned int x_pos = rects[c - ' '].x;
        unsigned int y_pos = rects[c - ' '].y;
        unsigned int height = rects[c - ' '].h;

        for(unsigned int bitmap_row_index = 0; bitmap_row_index < (unsigned int)(face->glyph->bitmap.rows); bitmap_row_index++)
        {
            for(unsigned int bitmap_column_index = 0; bitmap_column_index < (unsigned int)((face->glyph->bitmap.width)); bitmap_column_index++)
            {
                unsigned int index = index_bufora(x_pos + bitmap_column_index, (y_pos + height - 1) - bitmap_row_index, TARGET_DIM);
                atlas_tmp_buffer[index] = face->glyph->bitmap.buffer[bitmap_column_index + bitmap_row_index*(int)(face->glyph->bitmap.width)];
            }
        }

        //CALCULATE GLYPH TEXTURE COORDINATES
        atlas_gl.glyph_map[c].u_coord_left = double(x_pos)/double(TARGET_DIM);
        atlas_gl.glyph_map[c].u_coord_right = double(x_pos + face->glyph->bitmap.width)/double(TARGET_DIM);
        atlas_gl.glyph_map[c].v_coord_top = double(y_pos + face->glyph->bitmap.rows)/double(TARGET_DIM);
        atlas_gl.glyph_map[c].v_coord_bottom = double(y_pos)/double(TARGET_DIM);

        //        cout << "ZNAK = " << c << endl;
        //        cout << "texture_offset = " << texture_u_offset << endl;
        //        cout << "glyph->bitmap.width = " << atlas_gl.glyph_map[c].glyph_bitmap_width << endl;
        //        cout << "glyph->bitmap.rows = " << atlas_gl.glyph_map[c].glyph_bitmap_rows << endl;
        //        cout <<  "charactersMap[c].u_coord_left = " << atlas_gl.glyph_map[c].u_coord_left << endl;
        //        cout << "charactersMap[c].u_coord_right = " << atlas_gl.glyph_map[c].u_coord_right << endl;

        pen += face->glyph->bitmap.width;
    }

    atlas_gl.atlas_width = TARGET_DIM;
    atlas_gl.atlas_rows = TARGET_DIM;

    glGenTextures(1, &atlas_gl.square_AtlasTextureId);
    glBindTexture(GL_TEXTURE_2D,  atlas_gl.square_AtlasTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlas_gl.atlas_width, atlas_gl.atlas_rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, atlas_tmp_buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture (GL_TEXTURE_2D, 0);

    delete[] atlas_tmp_buffer;
    free(rects);
}

void TextRenderer_v2::debug_RenderSquareAtlas(GLuint vbo, GLfloat x, GLfloat y)
{
    string text = "Atlas";

    GLfloat pen_x = 50;
    GLfloat pen_y = 200;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glUseProgram(shader_program);
    {
        glUniform4fv(textColourLocation,1,glm::value_ptr(mTextColour));
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mProjection));
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mView));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(textureMapLocation, 0);
        glBindTexture(GL_TEXTURE_2D, current_atlas->square_AtlasTextureId);

        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(position_location);

        glVertexAttribPointer(texCoord_attrib_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(texCoord_attrib_location);


        //VERTICLE LEFT TOP 0
        verticles_table[0] = pen_x;    //verticle x pos
        verticles_table[1] = pen_y + current_atlas->atlas_rows;     //verticle y pos
        //verticles_table[2 + index] = 0.0f;    //verticle z pos
        verticles_table[3] = 0.0f; //U
        verticles_table[4] = 1.0f; //V

        //VERTICLE LEFT BOTTOM 1
        verticles_table[5] = pen_x;    //verticle x pos
        verticles_table[6] = pen_y;  //verticle y pos
        //verticles_table[7 + index] = 0.0f;    //verticle z pos
        verticles_table[8] = 0.0f; //U
        verticles_table[9] = 0.0f; //V

        //VERTICLE RIGH TOP 2
        verticles_table[10] = pen_x + current_atlas->atlas_width;  //verticle x pos
        verticles_table[11] = pen_y + current_atlas->atlas_rows;    //verticle y pos
        //verticles_table[12 + index] = 0.0f;    //verticle z pos
        verticles_table[13] = 1.0f; //U
        verticles_table[14] = 1.0f; //V

        //VERTICLE RIGHT BOTTOM 3
        verticles_table[15] = pen_x + current_atlas->atlas_width;  //verticle x pos
        verticles_table[16] = pen_y; //verticle y pos
        //verticles_table[17 + index] = 0.0f; //verticle z pos
        verticles_table[18] = 1.0f; //U
        verticles_table[19] = 0.0f; //V

        glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(verticles_table), verticles_table);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindTexture (GL_TEXTURE_2D, 0);

    }
    glUseProgram(0);
}

void TextRenderer_v2::setColour(glm::vec4 colour)
{
    mTextColour = colour;
}

void TextRenderer_v2::setCustomPV(glm::mat4 P, glm::mat4 V)
{
    mProjection = P;
    mView = V;
    useCustomPV = true;
}


void TextRenderer_v2::RenderText(std::string text,  GLfloat x, GLfloat y, TextPosition origin)
{
    glm::mat4 Model = glm::translate(glm::mat4(1),glm::vec3(round(x), round(y), 0));
    RenderText(1, text,Model,origin);
}

void TextRenderer_v2::RenderText(uint8_t instance, std::string text,  GLfloat x, GLfloat y, TextPosition origin)
{
    glm::mat4 Model = glm::translate(glm::mat4(1),glm::vec3(round(x), round(y), 0));
    RenderText(instance, text,Model,origin);
}

void TextRenderer_v2::RenderText(uint8_t instance, std::string text, glm::mat4 model, TextPosition origin)
{
    TextInstance *textInstance = &(vboBuffersMap[instance]);
    if(textInstance->vbo == 0)
    {
        GLuint new_vbo = prepareVBO(verticles_table, sizeof(verticles_table));
        textInstance->vbo = new_vbo;
    }

    GLuint vbo = textInstance->vbo;
    //debug_RenderSquareAtlas(vbo, 0, 0);
    //return;

    //cout << "Use VBO = " << vbo << endl;


    GLfloat pen_x_float = 0;
    int pen_x_int = 0;
    GLfloat pen_y_float = 0;
    GLfloat x_left = 0.0f;
    GLfloat x_right = 0.0f;
    GLfloat y_top = 0.0f;
    GLfloat y_bottom = 0.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glUseProgram(shader_program);
    {

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(textureMapLocation, 0);
        glBindTexture(GL_TEXTURE_2D, current_atlas->square_AtlasTextureId);

        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(position_location);

        glVertexAttribPointer(texCoord_attrib_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(texCoord_attrib_location);


        int index = 0;

        if(textInstance->previous_string != text){ // Optymalization - update buffers only when text changed
            textInstance->previous_string = text;
            // Iterate through all characters
            std::string::const_iterator c;

            Atlas_gl::GlyphData atlasCharData_tmp;

            for (c = text.begin(); c != text.end(); c++)
            {
                atlasCharData_tmp = current_atlas->glyph_map[*c];

                x_left = pen_x_int + atlasCharData_tmp.glyph_bitmap_left;
                x_right = x_left + atlasCharData_tmp.glyph_bitmap_width;
                y_top = pen_y_float + atlasCharData_tmp.glyph_bitmap_top;
                y_bottom = y_top - atlasCharData_tmp.glyph_bitmap_rows;

//                cout << " FOR LETTER " << *c << endl;

//                cout << "   DIMMENSIONS:" << endl;
//                cout << "   atlasCharData_tmp.glyph_bitmap_left = " << atlasCharData_tmp.glyph_bitmap_left << endl;
//                cout << "   atlasCharData_tmp.glyph_bitmap_width = " << atlasCharData_tmp.glyph_bitmap_width << endl;
//                cout << "   atlasCharData_tmp.glyph_bitmap_top = " << atlasCharData_tmp.glyph_bitmap_top << endl;
//                cout << "   atlasCharData_tmp.glyph_bitmap_rows = " << atlasCharData_tmp.glyph_bitmap_rows << endl;


//                cout << "  POSIOTIONS: " << endl;
//                cout << "   x_left = " << x_left << endl;
//                cout << "   x_right = " << x_right << endl;
//                cout << "   y_top = " << y_top << endl;
//                cout << "   y_bottom = " << y_bottom << endl;


                //                if(doOptymalization_1(x + x_left,y + y_top,y + y_bottom)){
                //                    break;
                //                }else if(doOptymalization_2(x + x_right)){
                //                    continue;
                //                }

                //VERTICLE LEFT TOP 0
                verticles_table[0 + index] = x_left;    //verticle x pos
                verticles_table[1 + index] = y_top;     //verticle y pos
                //verticles_table[2 + index] = 0.0f;    //verticle z pos
                verticles_table[3 + index] = atlasCharData_tmp.u_coord_left;
                verticles_table[4 + index] = atlasCharData_tmp.v_coord_top;

                //VERTICLE LEFT BOTTOM 1
                verticles_table[5 + index] = x_left;    //verticle x pos
                verticles_table[6 + index] = y_bottom;  //verticle y pos
                //verticles_table[7 + index] = 0.0f;    //verticle z pos
                verticles_table[8 + index] = atlasCharData_tmp.u_coord_left;
                verticles_table[9 + index] = atlasCharData_tmp.v_coord_bottom;

                //VERTICLE RIGH TOP 2
                verticles_table[10 + index] = x_right;  //verticle x pos
                verticles_table[11 + index] = y_top;    //verticle y pos
                //verticles_table[12 + index] = 0.0f;    //verticle z pos
                verticles_table[13 + index] = atlasCharData_tmp.u_coord_right;
                verticles_table[14 + index] = atlasCharData_tmp.v_coord_top;

                //VERTICLE RIGHT BOTTOM 3
                verticles_table[15 + index] = x_right;  //verticle x pos
                verticles_table[16 + index] = y_bottom; //verticle y pos
                //verticles_table[17 + index] = 0.0f; //verticle z pos
                verticles_table[18 + index] = atlasCharData_tmp.u_coord_right;
                verticles_table[19 + index] = atlasCharData_tmp.v_coord_bottom;


                pen_x_float += ((GLfloat)(atlasCharData_tmp.glyph_advance_x));
                pen_x_int = pen_x_float/64.0f;
                index += 20;
            }
            textInstance->textLenght = x_right;
            cout << "Text Lenght = " << textInstance->textLenght << endl;
            cout << "UPDATE VBO !!!!" << endl;
            glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(verticles_table), verticles_table);
        }



        //        static float angle = 0.0f;
        //        angle = angle + 0.15f;
        //        model = glm::rotate(model, glm::radians(angle), glm::vec3(0,0,1.0));

        //        static float scale = 1.0f;
        //        static float time = 0;
        //        time = time + 0.001;
        //        scale = cos(time)/4.0f + 0.5;

        //        model = glm::scale(model, glm::vec3(scale, scale, 1.0));


        if(origin == TEXT_RIGHT)
            ;
        else if(origin == TEXT_CENTER)
            model = glm::translate(model, glm::vec3(round(- textInstance->textLenght/2.0f), 0, 0));
        else if(origin == TEXT_LEFT)
            model = glm::translate(model, glm::vec3(round(- textInstance->textLenght), 0, 0));


        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mProjection));
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mView));

        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model));

        glUniform4fv(textColourLocation,1,glm::value_ptr(mTextColour));

        glDrawElements(GL_TRIANGLES, 6*text.size(), GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindTexture (GL_TEXTURE_2D, 0);
    }
    glUseProgram(0);
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

GLuint TextRenderer_v2::prepareVBO(const GLfloat * data, GLsizeiptr size){
    GLuint vbo;

    glGenBuffers(1,&vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(position_location);

        glVertexAttribPointer(texCoord_attrib_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(texCoord_attrib_location);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

bool TextRenderer_v2::doOptymalization_2(GLfloat x_right){
    if(x_right < 0.0f){
        return true;
    }
    return false;
}

bool TextRenderer_v2::doOptymalization_1(GLfloat  x_left, GLfloat y_top, GLfloat y_bottom ){
    if((x_left > viewport.z) || (y_bottom > viewport.w) || (y_top < 0.0f)){
        return true;
    }
    return false;
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

