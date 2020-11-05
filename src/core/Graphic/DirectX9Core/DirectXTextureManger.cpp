//
// Created by EunwooSong on 2020-11-05.
//

#include "include/Engine/SystemManager/DirectXTextureManger.h"
#include "include/Graphic/DirectX9Core/DirectXCore.h"

using namespace SharkEngine::Core;

DirectXTextureManger::~DirectXTextureManger() {
    for(auto iter : textureMap)
        SAFE_DELETE(iter.second);
}
LPDIRECT3DTEXTURE9 *DirectXTextureManger::LoadTextureFormFile(const char *path) {
    if (textureMap[path] == NULL) {
        textureMap[path] = new LPDIRECT3DTEXTURE9();
        D3DXCreateTextureFromFileExA(
                DIRECTX_CORE->GetDirect3DDevice(),
                path,
                D3DX_DEFAULT_NONPOW2,
                D3DX_DEFAULT_NONPOW2,
                1,
                0,
                D3DFMT_UNKNOWN,
                D3DPOOL_MANAGED,
                D3DX_DEFAULT,
                D3DX_DEFAULT,
                NULL,
                NULL,
                NULL,
                *(&textureMap[path])
        );
    }

    return textureMap[path];
}
