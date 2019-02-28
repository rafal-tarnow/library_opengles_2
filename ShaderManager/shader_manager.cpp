#include "shader_manager.hpp"

#include <SOIL.h>

ShaderManager ShaderManager::instance;

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{
    auto it = mapaShaderow.begin();

    while(it != mapaShaderow.end())
    {
        Shader_m * shader = it->second;
        delete shader;
        it++;
    }
}

ShaderManager * ShaderManager::getInstance()
{
    return & instance;
}

Shader_m * ShaderManager::getShaderFromFile(string filename)
{

}

Shader_m * ShaderManager::getShaderFromSource(string file_name, string vertex_source, string fragment_source)
{
    Shader_m * shader = mapaShaderow[file_name];

    if(shader)
    {
        return shader;
    }
    else
    {
        shader = new Shader_m(vertex_source, fragment_source, Shader_m::SOURCE);
        mapaShaderow[file_name] = shader;
        return shader;
    }


}

