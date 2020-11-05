//
// Created by EunwooSong on 2020-11-05.
//

#ifndef SHARKENGINE_SPRITE2DRENDERER_H
#define SHARKENGINE_SPRITE2DRENDERER_H

#include "../../CoreDefine.h"
#include "Base/Component.h"

namespace SharkEngine::Core {
    class Sprite2DRenderer
         : public Component {
    public:
        Sprite2DRenderer(){}
        Sprite2DRenderer(const char* path);
        Sprite2DRenderer(std::string path);
        ~Sprite2DRenderer(){}

        void SetTexture(const char * path);

        void Render() override;

        int GetWidth()          { return width; }
        int GetHeight()         { return height; }
        Vec2 GetTextureSize()   { return Vec2(width, height); }
        D3DXCOLOR GetColor()    { return color; }
        RECT GetVisibleRect()   { return visibleRect; }

        void SetColor(D3DXCOLOR color) {this->color = color;}
        void SetTexture(LPDIRECT3DTEXTURE9* texture) {this->texture = texture;}

    protected:
        LPDIRECT3DTEXTURE9* texture;
        D3DXCOLOR color;
        RECT visibleRect;
        int width;
        int height;
    };
}

#endif//SHARKENGINE_SPRITE2DRENDERER_H
