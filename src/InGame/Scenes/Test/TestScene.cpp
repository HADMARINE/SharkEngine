//
// Created by EunwooSong on 2020-10-30.
//

#include "TestScene.h"
#include "InGame/GameObjects/TEst/TestObject.h"
#include "InGame/GameObjects/TEst/TestUIObject.h"
#include "InGame/Scripts/Test/TestComponent.h"

#include "InGame/GameObjects/FloorObject.hpp"

using namespace SharkEngine;

void TestScene::Init() {
    Scene::Init();
    this->RegisterComponent<TestComponent>();
    this->RegisterComponent<FloorComponent>();

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
