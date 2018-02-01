#include "texture_manager.hpp"

#include <SOIL.h>

map<string, GLuint> TextureManager::mapaTesktur;

GLuint TextureManager::getTextureId(string fileName){
    if(mapaTesktur.count(fileName) == 1){
        //jeżeli istnieje juz taka teksuta to ją zwróc
        return mapaTesktur.at(fileName);
    }else{
        //w przeciwnym wypadku utworz nowa teksture
        mapaTesktur[fileName] = SOIL_load_OGL_texture(fileName.c_str(), 4,0,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
        return mapaTesktur.at(fileName);
    }
    return 0;
}


void TextureManager::deleteAllTextures(){
    //TODO zrobic usuwanie tekstur
      //glDeleteTextures(1)
}
