#include "TextRenderer_v2.hpp"

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;

static GLuint shader_program;

static GLint compileShaders(const char *vertex_shader_source, const char *fragment_shader_source);
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
        "   gl_FragColor = /*vec4(0.5,0.5,0.5,0.3) +*/ vec4(1.0,1.0,0.0,1.0) * sampled ;     \n"
        "   //gl_FragColor = vec4(1.0,1.0,1.0,1.0);               \n"
        "}                                                      \n";



TextRenderer_v2::TextRenderer_v2(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels){

    current_viewport_width_in_pixels = viewport_width_in_pixels;
    current_viewport_height_in_pixels = viewport_height_in_pixels;


    shader_program = compileShaders(vertex_shader_source, fragment_shader_source);

    position_location = glGetAttribLocation(shader_program, "position");
    texCoord_attrib_location = glGetAttribLocation(shader_program,"texCoord");
    textureUnitLocation = glGetUniformLocation (shader_program, "textureUnit" );



    projectionMatrixLocation = glGetUniformLocation(shader_program, "projection");
    glUseProgram(shader_program);
    {
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::ortho(static_cast<GLfloat>(0), static_cast<GLfloat>(viewport_width_in_pixels), static_cast<GLfloat>(0), static_cast<GLfloat>(viewport_height_in_pixels))));
    }
    glUseProgram(0);

    vbo = prepareVBO(verticles_table, sizeof(verticles_table));


    const int TEXT_BUFFER_SIZE = 256;
    TextData.textBuffer = new char[TEXT_BUFFER_SIZE];
    TextData.textBuffer = "Tekst do wyswietlenia";

    glGenTextures(1, &(TextData.textBufferTexture));
    glBindTexture(GL_TEXTURE_2D, TextData.textBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, TEXT_BUFFER_SIZE, 1, 0, GL_ALPHA, GL_UNSIGNED_BYTE, TextData.textBuffer);
    glBindTexture (GL_TEXTURE_2D, 0);




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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




}

TextRenderer_v2::~TextRenderer_v2(){
    glDeleteBuffers(1, &vbo);
}

void TextRenderer_v2::RenderText(std::string text,  GLfloat x, GLfloat y){

    //text = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";
    //text = "QWERTFSDAFASDFASDFASDFAEDFGASDFASDFASDFSDFAfasdfasdfDFSGADFGAYdfgasdfg";


    //text = "ABCEFGHIJASDFGASDFGASDFASDFASDFASDFKLMNOPQRASTUfasdfaWXYZ";

    //cout << "text.size() = " << text.size() << endl;

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

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

<<<<<<< HEAD
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(textureUnitLocation, 1);
        glBindTexture(GL_TEXTURE_2D, GlyphAtlasData.glyphAtlasTextureId);
=======
        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(position_location);

        glVertexAttribPointer(texCoord_attrib_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(texCoord_attrib_location);

        //glActiveTexture(GL_TEXTURE0);
        //glUniform1i(textureUnitLocation, GL_TEXTURE0);
>>>>>>> 3b018f1d22dcc413b5058003badaef475fd91ef1


        int index = 0;

        if(previous_string != text){ // Optymalization - update buffers only when text changed
            previous_string = text;
            // Iterate through all characters
            std::string::const_iterator c;
            for (c = text.begin(); c != text.end(); c++)
            {
                charData_tmp = charactersMap[*c];

                x_left = pen_x + charData_tmp.glyph_bitmap_left;
                x_right = x_left + charData_tmp.glyph_bitmap_width;
                y_top = pen_y + charData_tmp.glyph_bitmap_top;
                y_bottom = y_top - charData_tmp.glyph_bitmap_rows;

                if(doOptymalization_1(x_left, y_top, y_bottom)){
                    break;
                }else if(doOptymalization_2(x_right)){
                    continue;
                }



                //LEFT TOP 0
                verticles_table[0 + index] = x_left;
                verticles_table[1 + index] = y_top;
                verticles_table[3 + index] = charData_tmp.u_coord_left;
                verticles_table[4 + index] = 0.0f;//charData_tmp.v_coord_top;
                //LEFT BOTTOM 1
                verticles_table[5 + index] = x_left;
                verticles_table[6 + index] = y_bottom;
                verticles_table[8 + index] = charData_tmp.u_coord_left;
                verticles_table[9 + index] = 1.0f;//charData_tmp.v_coord_bottom;

                //RIGH TOP 2
                verticles_table[10 + index] = x_right;
                verticles_table[11 + index] = y_top;
                verticles_table[13 + index] = charData_tmp.u_coord_right;
                verticles_table[14 + index] = 0.0f;//charData_tmp.v_coord_top;

                //RIGHT BOTTOM 3
                verticles_table[15 + index] = x_right;
                verticles_table[16 + index] = y_bottom;
                verticles_table[18 + index] = charData_tmp.u_coord_right;
                verticles_table[19 + index] = 1.0f;//charData_tmp.v_coord_bottom;






                //            if(*c == '!'){
                //                cout << "**** DRAW ENGINE ****" << endl;
                //                cout << " znak = " << *c << endl;
                //                for(unsigned int i =0; i < 20; i++ ){
                //                    cout << "verticles_table[" << i << "] = " << verticles_table[i] << endl;
                //                }
                //                cout << "*********************" << endl;
                //            }



                pen_x += charData_tmp.glyph_advance_x;
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

void TextRenderer_v2::onVievportResize(GLfloat viewport_width_in_pixels, GLfloat viewport_height_in_pixels){

    current_viewport_width_in_pixels = viewport_width_in_pixels;
    current_viewport_height_in_pixels = viewport_height_in_pixels;

    projectionMatrixLocation = glGetUniformLocation(shader_program, "projection");
    glUseProgram(shader_program);
    {
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::ortho(static_cast<GLfloat>(0), static_cast<GLfloat>(current_viewport_width_in_pixels), static_cast<GLfloat>(0), static_cast<GLfloat>(current_viewport_height_in_pixels))));
    }
    glUseProgram(0);
}

void TextRenderer_v2::Load(std::string font, GLuint fontSize){


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

    if(FT_Select_Charmap(face, FT_ENCODING_UNICODE ))
        std::cout << "ERROR Select Charmap" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint TextureID;

    unsigned int max_rows = 0;
    unsigned int max_width = 0;
    int total_width = 0;

    for(char c = ' '; c <= 'z'; c++)
    {

        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        drawGlyphToConsole(face);


        // Destroy FreeType once we're finished

        cout << "ZNAK: " << c << endl;
        cout << "face->glyph->advance.x = " << face->glyph->advance.x << endl;
        cout << "face->glyph->advance.x in pixels = " << face->glyph->advance.x/64 << endl;
        cout << "face->glyph->bitmap.width = " << face->glyph->bitmap.width << endl;
        cout << "face->glyph->bitmap.rows = " << face->glyph->bitmap.rows << endl;

        if(face->glyph->bitmap.rows > max_rows){
            max_rows = face->glyph->bitmap.rows;
        }
        if(face->glyph->bitmap.width > max_width){
            max_width = face->glyph->bitmap.width;
        }
        total_width += face->glyph->bitmap.width;


        charData_tmp.glyph_bitmap_width = (GLfloat)(face->glyph->bitmap.width);
        charData_tmp.glyph_bitmap_rows = (GLfloat)(face->glyph->bitmap.rows);
        charData_tmp.glyph_bitmap_left = (GLfloat)(face->glyph->bitmap_left);
        charData_tmp.glyph_bitmap_top = (GLfloat)(face->glyph->bitmap_top);
        charData_tmp.glyph_advance_x = ((GLfloat)(face->glyph->advance.x))/64.0f;
        charData_tmp.glyph_bitmap_buffer = new unsigned char[int((face->glyph->bitmap.width)*(face->glyph->bitmap.rows))];
        for(unsigned int i = 0; i < (face->glyph->bitmap.width)*(face->glyph->bitmap.rows); i++){
            charData_tmp.glyph_bitmap_buffer[i] = face->glyph->bitmap.buffer[i];
        }


        glGenTextures(1, &TextureID);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, charData_tmp.glyph_bitmap_width, charData_tmp.glyph_bitmap_rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, charData_tmp.glyph_bitmap_buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture (GL_TEXTURE_2D, 0);

        charData_tmp.characterTextureID = TextureID;
        charactersMap[c] = charData_tmp;
    }




    cout << "Glyph count = " << charactersMap.size() << endl;
    cout << "Max rows = " << max_rows << endl;
    cout << "Max width = " << max_width << endl;
    cout << "Total_width = " << total_width << endl;

    char c = '}';


    prepareGlypAtlas(total_width, max_rows);







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

GLuint TextRenderer_v2::prepareVBO(const GLfloat * data, GLsizeiptr size){
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

void TextRenderer_v2::prepareGlypAtlas(unsigned int total_width, unsigned int max_rows){
    CharacterData characterData_nawias_glyph;

    characterData_nawias_glyph.glyph_bitmap_width = total_width;
    characterData_nawias_glyph.glyph_bitmap_rows = max_rows;
    characterData_nawias_glyph.glyph_bitmap_left = 0;
    characterData_nawias_glyph.glyph_bitmap_top = max_rows;
    characterData_nawias_glyph.glyph_advance_x = max_rows;
    characterData_nawias_glyph.glyph_bitmap_buffer = new unsigned char[(int)((total_width)*(max_rows))];

    int pen = 0;
    CharacterData tmp_CharacterData;
    for(char c = ' '; c <= 'z'; c++)
    {

        tmp_CharacterData = charactersMap[c];
        for(unsigned int j = 0; j < (unsigned int)(tmp_CharacterData.glyph_bitmap_rows); j++){
            for(unsigned int i = 0; i < (unsigned int)((tmp_CharacterData.glyph_bitmap_width)); i++){
                characterData_nawias_glyph.glyph_bitmap_buffer[pen+ i + j*total_width] = tmp_CharacterData.glyph_bitmap_buffer[i + j*(int)(tmp_CharacterData.glyph_bitmap_width)];
            }
        }





        charactersMap[c].u_coord_left = ((GLfloat)pen/(GLfloat)total_width);
        pen += tmp_CharacterData.glyph_bitmap_width;
        charactersMap[c].u_coord_right = ((GLfloat)pen/(GLfloat)total_width);

        cout << "ZNAK = " << c << endl;
        cout <<  "charactersMap[c].u_coord_left = " << charactersMap[c].u_coord_left << endl;
        cout << "charactersMap[c].u_coord_right = " << charactersMap[c].u_coord_right << endl;


    }

    glGenTextures(1, & GlyphAtlasData.glyphAtlasTextureId);
    glBindTexture(GL_TEXTURE_2D,  GlyphAtlasData.glyphAtlasTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, characterData_nawias_glyph.glyph_bitmap_width, characterData_nawias_glyph.glyph_bitmap_rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, characterData_nawias_glyph.glyph_bitmap_buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture (GL_TEXTURE_2D, 0);

    characterData_nawias_glyph.characterTextureID =  GlyphAtlasData.glyphAtlasTextureId;
    charactersMap['}'] = characterData_nawias_glyph;
}

bool TextRenderer_v2::doOptymalization_2(GLfloat x_right){
    if(x_right < 0.0f){
        return true;
    }
    return false;
}

bool TextRenderer_v2::doOptymalization_1(GLfloat  x_left, GLfloat y_top, GLfloat y_bottom ){
    if((x_left > current_viewport_width_in_pixels) || (y_bottom > current_viewport_height_in_pixels) || (y_top < 0.0f)){
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

