#pragma once

#include <GLES2/gl2.h>

#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;



class TextureManager {
public:
    static TextureManager * getInstance();
    GLuint getTextureId(string filename);
    GLuint getTextureId(string filename, unsigned int flags);
    glm::vec2 getTextureSize(GLuint tetureId);
protected:
    TextureManager();
    ~TextureManager();
private:
    static TextureManager instance;
    map<string , GLuint> mapaTesktur;
    map<GLuint, glm::vec2> mapaRozmiarow;
};
