#include "shader_manager.hpp"

#include <SOIL.h>

map<string, GLuint> ShaderManager::mapaShaderow;

Shader * ShaderManager::getTextureShader()
{
    return nullptr;
}

Shader * ShaderManager::getColourShader()
{
    return nullptr;
}

void ShaderManager::deleteAllShaders(){
    //TODO zrobic usuwanie tekstur
      //glDeleteTextures(1)
}
