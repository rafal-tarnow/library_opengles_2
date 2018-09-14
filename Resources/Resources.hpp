#pragma once

#ifdef __ANDROID__
#include <android/asset_manager.h>
#endif

#include <library_opengles_2/Debug/Debug.hpp>

#include <string>

using namespace std;

class Resource{
public:
    Resource(string path);
    ~Resource();
    unsigned char * getData();
    uint32_t getSize();


#ifdef __ANDROID__
    static void initAndroid(AAssetManager*	pAssetManager);
#endif

private:
#ifdef __ANDROID__
    static AAssetManager* m_pAssetManager;
#endif
    unsigned char * data = NULL;
    uint32_t size = 0;
    DBG_COUNT("Resource");

};
