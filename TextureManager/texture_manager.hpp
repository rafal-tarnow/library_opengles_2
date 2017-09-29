#pragma once

#ifdef BACKEND_OPEN_GL_ES
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#else
#include <GL/gl.h>
#endif

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
