//
// Created by EunwooSong on 2020-11-05.
//

#ifndef SHARKENGINE_DIRECTXTEXTUREMANGER_H
#define SHARKENGINE_DIRECTXTEXTUREMANGER_H

#include <map>
#include "include/Engine/CoreDefine.h"

namespace SharkEngine::Core {
    class DirectXTextureManger {
    private:
        std::map<const char *, LPDIRECT3DTEXTURE9*> textureMap;

    public:
        DirectXTextureManger(){};
        ~DirectXTextureManger();

        LPDIRECT3DTEXTURE9* LoadTextureFormFile(const char* path);
    };
}

#endif//SHARKENGINE_DIRECTXTEXTUREMANGER_H
