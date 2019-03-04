#pragma once

#include <GLES2/gl2.h>

#include "../Shader/shader_m.h"
#include <string>
#include <map>

using namespace std;

class ShaderManager {
public:
    static ShaderManager * getInstance();
     Shader_m * getShaderFromFile(string filename);
     Shader_m * getShaderFromSource(string file_name, const GLchar*  vertex_source, const GLchar*  fragment_source);
protected:
    ShaderManager();
    ~ShaderManager();
private:
    static ShaderManager instance;
    map<string , Shader_m *> mapaShaderow;
};
