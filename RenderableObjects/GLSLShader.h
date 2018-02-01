#pragma once
#include <GLES2/gl2.h>
#include <map>
#include <string>

using namespace std;

class GLSLShader
{
public:
    GLSLShader(void);
    ~GLSLShader(void);
    void LoadFromString(GLenum whichShader, const std::string& source);
    void LoadFromFile(GLenum whichShader, const std::string& filename);
    void CreateAndLinkProgram();
    void Use();
    void UnUse();
    void AddAttribute(const std::string& attribute);
    void AddUniform(const std::string& uniform);

    //indekser zwracajšcy lokalizację atrybutu (uniformu)
    GLuint operator[](const std::string& attribute);
    GLuint operator()(const std::string& uniform);
    void DeleteShaderProgram();

private:
    enum ShaderType {VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};
    GLuint	_program;
    int _totalShaders;
    GLuint _shaders[3];//0->shader wierzchołków, 1->shader fragmentów, 2->shader geometrii
    std::map<std::string,GLuint> _attributeList;
    std::map<std::string,GLuint> _uniformLocationList;
};
