//
// Created by joonl on 2020-11-06.
//

#include "FloorComponent.hpp"

void FloorComponent::Update() {
    Component::Update();
    CLogger::Debug("%d Test Update", (int) m_Owner->GetEntityID());

    SHARK_LINE_MGR->DrawBox(GetOwner()->GetComponent<SharkEngine::Core::Transform>()->GetWorldPos(),
                            GetOwner()->GetComponent<SharkEngine::Core::Transform>()->GetWorldPos() +
                            GetOwner()->GetComponent<SharkEngine::Core::Sprite2DRenderer>()->GetTextureSize(),
                            D3DCOLOR_ARGB(255, 255, 0, 0));
}
