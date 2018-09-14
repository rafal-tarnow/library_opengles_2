#include "Resources.hpp"

#ifdef __ANDROID__
AAssetManager* Resource::m_pAssetManager;

void Resource::initAndroid(AAssetManager*	pAssetManager)
{
    m_pAssetManager = pAssetManager;
}
#endif

Resource::Resource(string path)
{
#ifdef __ANDROID__
    AAsset* asset=AAssetManager_open(m_pAssetManager,path.c_str(),AASSET_MODE_BUFFER);
    size = AAsset_getLength(asset);
    data = (unsigned char *)malloc(size);
    AAsset_read(asset, data, size);
    AAsset_close(asset);
#else
    path = "./assets/" + path;

    FILE * file  = fopen(path.c_str(), "r");
    fseek(file, 0, SEEK_END); // seek to end of file
    size = ftell(file); // get current file pointer
    fseek(file, 0, SEEK_SET);

    data = (unsigned char *)malloc(size);

    size_t result;
    fread (data,1,size,file);
    if (result != size) {/*fputs ("Reading error",stderr); exit (3);*/}

    fclose(file);
#endif
}

Resource::~Resource()
{
    free(data);
    data = NULL;
    size = 0;
}

unsigned char * Resource::getData()
{
    return data;
}
uint32_t Resource::getSize()
{
    return size;
}
