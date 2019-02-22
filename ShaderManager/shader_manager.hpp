#pragma once

#include <GLES2/gl2.h>

#include "../Shader/shader_m.h"
#include <string>
#include <map>

using namespace std;

class ShaderManager {
public:
    static Shader * getTextureShader();
    static Shader * getColourShader();
    static void deleteAllShaders();
private:
    static map<string , GLuint> mapaShaderow;
};
