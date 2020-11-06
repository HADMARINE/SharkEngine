//
// Created by EunwooSong on 2020-11-06.
//
#include <include/Engine/Scene/Components/BoxCollider.hpp>
#include <include/Engine/Scene/Components/Transform.h>

Vec2 SharkEngine::Core::BoxCollider::GetLeftTopPos(){
    Transform *transform = GetOwner()->GetComponent<Transform>();
    return transform->GetWorldPos();
}
Vec2 SharkEngine::Core::BoxCollider::GetRightBottomPos() {
    Transform *transform = GetOwner()->GetComponent<Transform>();
    return this->relativeRightBottomPos - this->relativeLeftTopPos + transform->GetWorldPos();
}

void SharkEngine::Core::BoxCollider::Update() {
    Component::Update();
}