//
// Created by EunwooSong on 2020-10-30.
//

#include "TestScene.h"
#include "InGame/GameObjects/TEst/TestObject.h"
#include "InGame/GameObjects/TEst/TestUIObject.h"
#include "InGame/Scripts/Test/TestComponent.h"
#include "include/Engine/Scene/Components/CameraComponent.h"
#include "include/Engine/Scene/Components/Transform.h"
#include "InGame/GameObjects/FloorObject.hpp"

using namespace SharkEngine;
using namespace SharkEngine::Core;

void TestScene::Init() {
    Scene::Init();
    this->RegisterComponent<TestComponent>();
    this->RegisterComponent<FloorComponent>();

    GameObject* obj = new GameObject(); obj->Init();
    obj->AddComponent<CameraComponent>();
    obj->GetComponent<Transform>()->SetScale(Vec2(0.1f, 0.1f));


    TestObject* tmp = new TestObject();
    FloorObject* floor = new FloorObject();
//    TestObject* tmp1 = new TestObject();
//    TestObject* tmp2 = new TestObject();

    tmp->Init();
    floor->Init();
//    tmp1->Init();
//    tmp2->Init();
    //tmp1->transform->SetLocalPos(Vec2(500, 500));
}
