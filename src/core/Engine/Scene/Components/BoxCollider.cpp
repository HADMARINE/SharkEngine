//
// Created by EunwooSong on 2020-11-06.
//
#include <include/Engine/Scene/Components/BoxCollider.hpp>
#include <include/Engine/Scene/Components/Transform.h>

Vec2 SharkEngine::Core::BoxCollider::GetLeftTopPos(){
    Transform *transform = GetOwner()->GetComponent<Transform>();
    return this->relativeLeftTopPos + transform->GetWorldPos();
}
Vec2 SharkEngine::Core::BoxCollider::GetRightBottomPos() {
    Transform *transform = GetOwner()->GetComponent<Transform>();
    return this->relativeRightBottomPos + transform->GetWorldPos();
}
