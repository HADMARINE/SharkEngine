//
// Created by EunwooSong on 2020-11-06.
//

#include "include/Engine/Scene/Components/CameraComponent.h"
#include "include/Engine/SharkSystem.hpp"
#include "include/Engine/Scene/Components/Transform.h"
void SharkEngine::Core::CameraComponent::Render() {
    auto mul = SHARK_SCENE_MGR->GetCurrentScene()->GetComponentArray<Transform>();
    D3DXMATRIX mat = GetOwner()->transform->GetMatrix();

    for(auto iter : mul) {
        iter->MulMatrix(mat);
    }
}
