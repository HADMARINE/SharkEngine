//
// Created by EunwooSong on 2020-11-04.
//

#include "../../../include/Graphic/DirectX9Core/DirectXCore.h"
#include "../../../GlobalPreferences.hpp"
#include "../../../include/Engine/CoreDefine.h"

void DirectXCore::Initialize(HWND hWnd) {
    p_d3d = Direct3DCreate9(D3D_SDK_VERSION);
    ZeroMemory(&p_d3d_params, sizeof(p_d3d_params));

    p_d3d_params.Windowed = TRUE;
    p_d3d_params.SwapEffect =  D3DSWAPEFFECT_DISCARD;

    p_d3d->CreateDevice(D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL,
                        hWnd,
                        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                        &p_d3d_params,
                        &p_d3d_device);

    D3DXCreateSprite(p_d3d_device, &p_d3d_sprite);
    D3DXCreateLine(p_d3d_device, &p_d3d_line);
}
void DirectXCore::Release() {
    SAFE_RELEASE(p_d3d_sprite);
    SAFE_RELEASE(p_d3d_device);
    SAFE_RELEASE(p_d3d);
}
void DirectXCore::BeginRender() {
    p_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
    p_d3d_device->BeginScene();
    
    p_d3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
    p_d3d_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
    p_d3d_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}
void DirectXCore::EndRender() {
    p_d3d_sprite->End();

    p_d3d_device->EndScene();
    p_d3d_device->Present(NULL, NULL, NULL, NULL);
}

DirectXCore *DirectXCore::Instance() {
    static DirectXCore* iter = new DirectXCore();

    return iter;
}
