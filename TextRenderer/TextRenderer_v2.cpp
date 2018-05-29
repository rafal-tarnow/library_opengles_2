#include "TextRenderer_v2.hpp"

#include <iostream>

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
        "//TextRenderer_v2 fragmet shader           \n"
        "precision mediump float;                   \n"
        "                                           \n"
        "varying vec2 v_TexCoordinate;              \n"
        "uniform sampler2D textureUnit;             \n"
        "uniform vec4 textColor;                    \n"
        "void main() {                              \n"
        "   vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(textureUnit,v_TexCoordinate).a);   \n"
        "   gl_FragColor = textColor * sampled ;     \n"
        "}                                                      \n";

GLuint TextRenderer_v2::shader_program = 0;
GLint TextRenderer_v2::position_location;
GLint TextRenderer_v2::texCoord_attrib_location;
GLint TextRenderer_v2::textureUnitLocation;
GLint TextRenderer_v2::textColourLocation;
GLint TextRenderer_v2::projectionMatrixLocation;
map<GLuint , Atlas_gl *> TextRenderer_v2::mapaAtlasow;


static GLint compileShaders(const char *vertex_shader_source, const char *fragment_shader_source);
static void drawGlyphToConsole(FT_Face &face);



TextRenderer_v2::TextRenderer_v2(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels, glm::vec4 txtColor)
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
        textureUnitLocation = glGetUniformLocation (shader_program, "textureUnit" );
        textColourLocation = glGetUniformLocation(shader_program, "textColor");
        projectionMatrixLocation = glGetUniformLocation(shader_program, "projection");
    }


    vbo = prepareVBO(verticles_table, sizeof(verticles_table));

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

TextRenderer_v2::~TextRenderer_v2(){
    glDeleteBuffers(1, &vbo);
}

void TextRenderer_v2::RenderText(std::string text,  GLfloat x, GLfloat y)
{
    GLfloat pen_x = x;
    GLfloat pen_y = y;
    GLfloat x_left = 0.0f;
    GLfloat x_right = 0.0f;
    GLfloat y_top = 0.0f;
    GLfloat y_bottom = 0.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glUseProgram(shader_program);
    {
        glUniform4fv(textColourLocation,1,glm::value_ptr(mTextColour));
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mProjection));

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(textureUnitLocation, 0);
        glBindTexture(GL_TEXTURE_2D, current_atlas->glyphAtlasTextureId);

        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(position_location);

        glVertexAttribPointer(texCoord_attrib_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(texCoord_attrib_location);


        int index = 0;

        if(previous_string != text){ // Optymalization - update buffers only when text changed
            previous_string = text;
            // Iterate through all characters
            std::string::const_iterator c;

            Atlas_gl::GlyphData atlasCharData_tmp;

            for (c = text.begin(); c != text.end(); c++)
            {
                atlasCharData_tmp = current_atlas->glyph_map[*c];

                x_left = pen_x + atlasCharData_tmp.glyph_bitmap_left;
                x_right = x_left + atlasCharData_tmp.glyph_bitmap_width;
                y_top = pen_y + atlasCharData_tmp.glyph_bitmap_top;
                y_bottom = y_top - atlasCharData_tmp.glyph_bitmap_rows;

                if(doOptymalization_1(x_left, y_top, y_bottom)){
                    break;
                }else if(doOptymalization_2(x_right)){
                    continue;
                }

                //VERTICLE LEFT TOP 0
                verticles_table[0 + index] = x_left;    //verticle x pos
                verticles_table[1 + index] = y_top;     //verticle y pos
                //verticles_table[2 + index] = 0.0f;    //verticle z pos
                verticles_table[3 + index] = atlasCharData_tmp.u_coord_left;
                verticles_table[4 + index] = 0.0f;//atlasCharData_tmp.v_coord_top;

                //VERTICLE LEFT BOTTOM 1
                verticles_table[5 + index] = x_left;    //verticle x pos
                verticles_table[6 + index] = y_bottom;  //verticle y pos
                //verticles_table[7 + index] = 0.0f;    //verticle z pos
                verticles_table[8 + index] = atlasCharData_tmp.u_coord_left;
                verticles_table[9 + index] = 1.0f;//atlasCharData_tmp.v_coord_bottom;

                //VERTICLE RIGH TOP 2
                verticles_table[10 + index] = x_right;  //verticle x pos
                verticles_table[11 + index] = y_top;    //verticle y pos
                //verticles_table[12 + index] = 0.0f;    //verticle z pos
                verticles_table[13 + index] = atlasCharData_tmp.u_coord_right;
                verticles_table[14 + index] = 0.0f;//atlasCharData_tmp.v_coord_top;

                //VERTICLE RIGHT BOTTOM 3
                verticles_table[15 + index] = x_right;  //verticle x pos
                verticles_table[16 + index] = y_bottom; //verticle y pos
                //verticles_table[17 + index] = 0.0f; //verticle z pos
                verticles_table[18 + index] = atlasCharData_tmp.u_coord_right;
                verticles_table[19 + index] = 1.0f;//atlasCharData_tmp.v_coord_bottom;


                pen_x += ((GLfloat)(atlasCharData_tmp.glyph_advance_x))/64.0f;
                index += 20;
            }
            glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(verticles_table), verticles_table);
        }

        //glDrawArrays(GL_TRIANGLE_STRIP, 0,4*text.size() );
        glDrawElements(GL_TRIANGLES, 6*text.size(), GL_UNSIGNED_INT, 0);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindTexture (GL_TEXTURE_2D, 0);

    }
    glUseProgram(0);

}

void TextRenderer_v2::onVievportResize(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels)
{
    viewport.z = viewport_width_in_pixels;
    viewport.w = viewport_height_in_pixels;

    mProjection = glm::ortho(static_cast<GLfloat>(0), static_cast<GLfloat>(viewport.z), static_cast<GLfloat>(0), static_cast<GLfloat>(viewport.w));
}

void TextRenderer_v2::LoadFromMemory(const unsigned char * font_data, int data_size, GLuint fontSize)
{
    if(mapaAtlasow.count(fontSize) == 1){
        //jeżeli atlas juz istnieje
        current_atlas = mapaAtlasow.at(fontSize);
        return;
    }else{
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

        prepareOpenGLAtlas(ft, face, fontSize, *atlas_gl);

        FT_Done_Face (face);
        FT_Done_FreeType (ft);

        mapaAtlasow[fontSize] = atlas_gl;
        current_atlas = atlas_gl;
    }
}

void TextRenderer_v2::Load(std::string font, GLuint fontSize)
{
    if(mapaAtlasow.count(fontSize) == 1){
        //jeżeli atlas juz istnieje
        current_atlas = mapaAtlasow.at(fontSize);
        return;
    }else{
        //w przeciwnym wypadku utworz nowy atlas

        FT_Library ft;

        if (FT_Init_FreeType(&ft))
        {
            std::cout << "[ERROR]::FREETYPE: Could not init FreeType Library" << std::endl;
            exit(-1);
        }

        FT_Face face;
        if (FT_New_Face(ft, font.c_str(), 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            exit(EXIT_FAILURE);
        }

        Atlas_gl * atlas_gl = new Atlas_gl();

        prepareOpenGLAtlas(ft, face, fontSize, *atlas_gl);

        FT_Done_Face (face);
        FT_Done_FreeType (ft);

        mapaAtlasow[fontSize] = atlas_gl;
        current_atlas = atlas_gl;
    }
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

void TextRenderer_v2::prepareOpenGLAtlas(FT_Library &ft, FT_Face &face, GLuint &fontSize, Atlas_gl &atlas_gl){


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
    }


    unsigned char *  atlas_tmp_buffer = new unsigned char[(int)((total_width)*(max_rows))];

    int pen = 0;
    for(char c = ' '; c <= 'z'; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        for(unsigned int bitmap_row_index = 0; bitmap_row_index < (unsigned int)(face->glyph->bitmap.rows); bitmap_row_index++)
        {
            for(unsigned int bitmap_column_index = 0; bitmap_column_index < (unsigned int)((face->glyph->bitmap.width)); bitmap_column_index++)
            {

                atlas_tmp_buffer[pen + bitmap_column_index + bitmap_row_index*total_width] = face->glyph->bitmap.buffer[bitmap_column_index + bitmap_row_index*(int)(face->glyph->bitmap.width)];
            }
        }

        atlas_gl.glyph_map[c].glyph_bitmap_width = face->glyph->bitmap.width;
        atlas_gl.glyph_map[c].glyph_bitmap_left = face->glyph->bitmap_left;
        atlas_gl.glyph_map[c].glyph_bitmap_top = face->glyph->bitmap_top;
        atlas_gl.glyph_map[c].glyph_bitmap_rows = face->glyph->bitmap.rows;
        atlas_gl.glyph_map[c].glyph_advance_x = face->glyph->advance.x;

        //CALCULATE GLYPH TEXTURE COORDINATES
        atlas_gl.glyph_map[c].u_coord_left = ((GLfloat)pen/(GLfloat)total_width);
        pen += face->glyph->bitmap.width;
        atlas_gl.glyph_map[c].u_coord_right = ((GLfloat)pen/(GLfloat)total_width);
        atlas_gl.glyph_map[c].v_coord_top = 1.0f;
        atlas_gl.glyph_map[c].v_coord_bottom = 1.0f - (GLfloat)face->glyph->bitmap.rows/(GLfloat)max_rows;
        cout << "ZNAK = " << c << endl;
        cout <<  "charactersMap[c].u_coord_left = " << atlas_gl.glyph_map[c].u_coord_left << endl;
        cout << "charactersMap[c].u_coord_right = " << atlas_gl.glyph_map[c].u_coord_right << endl;
    }

    glGenTextures(1, &atlas_gl.glyphAtlasTextureId);
    glBindTexture(GL_TEXTURE_2D,  atlas_gl.glyphAtlasTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, total_width, max_rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, atlas_tmp_buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture (GL_TEXTURE_2D, 0);

    delete[] atlas_tmp_buffer;
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

