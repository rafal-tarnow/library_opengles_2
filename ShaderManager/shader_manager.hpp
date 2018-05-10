#pragma once

#include <GLES2/gl2.h>

#include <string>
#include <map>

using namespace std;

class ShaderManager {
public:
    static GLuint getShaderId(string filename);
    static void deleteAllShaders();
private:
    static map<string , GLuint> mapaShaderow;
};
