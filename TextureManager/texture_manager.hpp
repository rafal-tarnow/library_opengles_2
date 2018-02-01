#pragma once

#include <GLES2/gl2.h>

#include <string>
#include <map>

using namespace std;

class TextureManager {
public:
    static GLuint getTextureId(string filename);
    static void deleteAllTextures();
private:
    static map<string , GLuint> mapaTesktur;
};
