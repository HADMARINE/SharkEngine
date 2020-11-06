//
// Created by EunwooSong on 2020-11-06.
//

#include "include/Engine/Scene/Components/CameraComponent.h"
#include "include/Engine/SharkSystem.hpp"
void SharkEngine::Core::CameraComponent::Render() {
    auto mul = SHARK_SCENE_MGR->GetCurrentScene()->GetComponentArray<Transform>();

    for(auto iter : mul) {
        
    }
}
