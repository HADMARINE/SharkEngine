//
// Created by EunwooSong on 2020-11-01.
//

#include "TestObject.h"
#include "../Scripts/TestComponent.h"
#include "../../include/Engine/Scene/Scene.h"
#include "include/Engine/Scene/Components/Sprite2DRenderer.h"
#include "include/Engine/Scene/Components/RigidBody2D.hpp"
#include "../../CLogger.hpp"
using namespace SharkEngine::Core;

void TestObject::Init() {
    GameObject::Init();

    CLogger::Info("Test Object Initialize");

    testComponent = new TestComponent();
    m_Scene->AddComponent<TestComponent>(this, testComponent);

    Sprite2DRenderer* tmp = AddComponent<Sprite2DRenderer>();
    tmp->SetTexture("source/viking_room.png");
//    BoxCollider* boxCollider = AddComponent<BoxCollider>();
    RigidBody2D* rigidBody = AddComponent<RigidBody2D>();
};
