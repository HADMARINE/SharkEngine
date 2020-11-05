//
// Created by EunwooSong on 2020-10-30.
//

#include "TestScene.h"
#include "../Scripts/TestComponent.h"
#include "../GameObjects/TestObject.h"

using namespace SharkEngine;

void TestScene::Init() {
    Scene::Init();
    this->RegisterComponent<TestComponent>();

    TestObject* tmp = new TestObject();
//    TestObject* tmp1 = new TestObject();
//    TestObject* tmp2 = new TestObject();

    tmp->Init();
//    tmp1->Init();
//    tmp2->Init();

    //DestroyComponent<TestObject>(tmp2->GetEntityID());

    //if(GetComponent<TestObject>(tmp2->GetEntityID()) == nullptr)
    //    CLogger::Error("real bug.!");
}
