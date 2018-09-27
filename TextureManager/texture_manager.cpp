#include "texture_manager.hpp"
#include "../Resources/Resources.hpp"

#include <SOIL.h>

TextureManager TextureManager::instance;

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
    auto it = mapaTesktur.begin();

    while (it != mapaTesktur.end())
    {
        GLuint textureId = it->second;
        glDeleteTextures(1,&textureId);
        it++;
    }
}

TextureManager * TextureManager::getInstance()
{
    return & instance;
}

GLuint TextureManager::getTextureId(string fileName){
    return getTextureId(fileName, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y );
}

GLuint TextureManager::getTextureId(string fileName, unsigned int flags)
{
    GLuint texture = mapaTesktur[fileName];

    if(glIsTexture(texture) == GL_TRUE){
        //jeżeli istnieje juz taka tektura
        return texture;
    }else{
        Resource imageResource(fileName);

        int width;
        int height;
        int channels;

        unsigned char *data = SOIL_load_image_from_memory(imageResource.getData(), imageResource.getSize(), &width, &height, &channels, SOIL_LOAD_AUTO);

        GLuint textureId = SOIL_create_OGL_texture(
                    data,
                    width, height, channels,
                    SOIL_CREATE_NEW_ID,
                    flags
                    );

        SOIL_free_image_data(data);

        mapaTesktur[fileName] = textureId;
        mapaRozmiarow[textureId] = glm::vec2(width, height);
        return mapaTesktur.at(fileName);
    }
    return 0;
}

glm::vec2 TextureManager::getTextureSize(GLuint textureId)
{
    return mapaRozmiarow[textureId];
}

