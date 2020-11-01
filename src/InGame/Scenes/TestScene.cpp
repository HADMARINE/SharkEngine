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

    tmp->Init();
}
