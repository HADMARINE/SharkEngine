//
// Created by EunwooSong on 2020-10-30.
//

#include "TestScene.h"
#include "../Scripts/TestComponent.h"
#include "../Scripts/FloorComponent.hpp"
#include "../GameObjects/TestObject.h"
#include "../GameObjects/FloorObject.hpp"

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

    //DestroyComponent<TestObject>(tmp2->GetEntityID());

    //if(GetComponent<TestObject>(tmp2->GetEntityID()) == nullptr)
    //    CLogger::Error("real bug.!");
}
