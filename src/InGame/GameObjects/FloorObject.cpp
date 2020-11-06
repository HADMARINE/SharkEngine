//
// Created by joonl on 2020-11-06.
//

#include "FloorObject.hpp"
#include "include/Engine/Scene/Components/RigidBody2D.hpp"
#include "include/Engine/Scene/Components/BoxCollider.hpp"



void FloorObject::Init() {
    using namespace SharkEngine::Core;
    GameObject::Init();


    floorComponent = new FloorComponent();
    m_Scene->AddComponent<FloorComponent>(this, floorComponent);

    Sprite2DRenderer* tmp = AddComponent<Sprite2DRenderer>();
    tmp->SetTexture("source/viking_room.png");
    RigidBody2D* rigidBody = AddComponent<RigidBody2D>();
    transform->SetLocalPos(Vec2(100, 300));
    rigidBody->SetGravity(0.0f);
    BoxCollider* boxCollider = AddComponent<BoxCollider>();
    boxCollider->SetLeftTopPos(transform->GetLocalPos());
    boxCollider->SetRightBottomPos(transform->GetLocalPos() + Vec2(1024, 1024) * transform->GetScale().x);
}
