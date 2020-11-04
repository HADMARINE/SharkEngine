//
// Created by EunwooSong on 2020-11-04.
//

#ifndef SHARKENGINE_DIRECTXCORE_H
#define SHARKENGINE_DIRECTXCORE_H

#include <d3d9.h>
#include <d3dx9.h>

#define DIRECTX_CORE DirectXCore::Instance()

class DirectXCore {
public:
    void Initialize(HWND hWnd);
    void Release();

    void BeginRender();
    void EndRender();

    static DirectXCore* Instance();

    LPDIRECT3D9 &GetDirect3D()              { return p_d3d; }
    D3DPRESENT_PARAMETERS &GetD3DParams()   { return p_d3d_params; }
    LPDIRECT3DDEVICE9 &GetDirect3DDevice()  { return p_d3d_device; }
    LPD3DXSPRITE &GetSprite()               { return p_d3d_sprite; }
    LPD3DXEFFECT &GetD3DEffect()            { return p_d3d_effect; }

private:
    LPDIRECT3D9             p_d3d;
    D3DPRESENT_PARAMETERS   p_d3d_params;
    LPDIRECT3DDEVICE9       p_d3d_device;
    LPD3DXSPRITE            p_d3d_sprite;
    LPD3DXEFFECT            p_d3d_effect;
};


#endif//SHARKENGINE_DIRECTXCORE_H
