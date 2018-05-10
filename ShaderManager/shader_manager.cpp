#include "shader_manager.hpp"

#include <SOIL.h>

map<string, GLuint> ShaderManager::mapaShaderow;

GLuint ShaderManager::getShaderId(string fileName){
    if(mapaShaderow.count(fileName) == 1){
        //jeżeli istnieje juz taka teksuta to ją zwróc
        return mapaShaderow.at(fileName);
    }else{
        //w przeciwnym wypadku utworz nowa teksture
        mapaShaderow[fileName] = SOIL_load_OGL_texture(fileName.c_str(), 4,0,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
        return mapaShaderow.at(fileName);
    }
    return 0;
}

void ShaderManager::deleteAllShaders(){
    //TODO zrobic usuwanie tekstur
      //glDeleteTextures(1)
}
