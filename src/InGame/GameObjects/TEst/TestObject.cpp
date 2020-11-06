//
// Created by EunwooSong on 2020-11-01.
//

#include "TestObject.h"
#include "CLogger.hpp"
#include "InGame/Scripts/Test/TestComponent.h"
#include "include/Engine/Scene/Components/Sprite2DRenderer.h"
<<<<<<< Updated upstream:src/InGame/GameObjects/TestObject.cpp
#include "include/Engine/Scene/Components/RigidBody2D.hpp"
#include "include/Engine/Scene/Components/BoxCollider.hpp"
#include "../../CLogger.hpp"
=======
#include "include/Engine/Scene/Scene.h"
>>>>>>> Stashed changes:src/InGame/GameObjects/TEst/TestObject.cpp
using namespace SharkEngine::Core;

void TestObject::Init() {
    GameObject::Init();

    CLogger::Info("Test Object Initialize");

    testComponent = new TestComponent();
    m_Scene->AddComponent<TestComponent>(this, testComponent);


//    transform->SetLocalPos(Vec2(1000, 1000));

    Sprite2DRenderer* tmp = AddComponent<Sprite2DRenderer>();
    tmp->SetTexture("source/viking_room.png");
//    BoxCollider* boxCollider = AddComponent<BoxCollider>();
    RigidBody2D* rigidBody = AddComponent<RigidBody2D>();
    BoxCollider* boxCollider = AddComponent<BoxCollider>();
    boxCollider->SetLeftTopPos(transform->GetLocalPos());
    boxCollider->SetRightBottomPos(transform->GetLocalPos() + Vec2(1024, 1024) * transform->GetScale().x);
};
