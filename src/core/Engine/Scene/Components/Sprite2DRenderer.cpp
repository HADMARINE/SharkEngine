//
// Created by EunwooSong on 2020-11-05.
//

#include "include/Engine/Scene/Components/Sprite2DRenderer.h"
#include "include/Engine/SharkSystem.hpp"
#include "include/Engine/Scene/Components/Transform.h"

SharkEngine::Core::Sprite2DRenderer::Sprite2DRenderer(const char *path) {
    SetTexture(path);
}
SharkEngine::Core::Sprite2DRenderer::Sprite2DRenderer(std::string path) {
    SetTexture(path.c_str());
}
void SharkEngine::Core::Sprite2DRenderer::SetTexture(const char * path) {
    texture = SHARK_TEXTURE_MGR->LoadTextureFormFile(path);

    D3DSURFACE_DESC desc;
    (*texture)->GetLevelDesc(0, &desc);

    width = desc.Width;
    height = desc.Height;

    visibleRect = Rect(0, 0, width, height);

    color = D3DCOLOR_ARGB(255, 255, 255, 255);
}

void SharkEngine::Core::Sprite2DRenderer::Render() {
    Component::Render();
    Transform* transform = m_Owner->GetScene()->GetComponent<SharkEngine::Core::Transform>(m_Owner->GetEntityID());
    SHARK_DIRECT3D->GetSprite()->SetTransform((const D3DXMATRIX *) transform->GetMatrix());
    SHARK_DIRECT3D->GetSprite()->Draw(*texture, &visibleRect, NULL, NULL, color);
}