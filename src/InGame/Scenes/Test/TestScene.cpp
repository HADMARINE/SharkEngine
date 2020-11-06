//
// Created by EunwooSong on 2020-10-30.
//

#include "TestScene.h"
<<<<<<< Updated upstream:src/InGame/Scenes/TestScene.cpp
#include "../Scripts/TestComponent.h"
#include "../Scripts/FloorComponent.hpp"
#include "../GameObjects/TestObject.h"
#include "../GameObjects/FloorObject.hpp"
=======
#include "InGame/GameObjects/TEst/TestObject.h"
#include "InGame/GameObjects/TEst/TestUIObject.h"
#include "InGame/Scripts/Test/TestComponent.h"
>>>>>>> Stashed changes:src/InGame/Scenes/Test/TestScene.cpp

using namespace SharkEngine;

void TestScene::Init() {
    Scene::Init();
    this->RegisterComponent<TestComponent>();
    this->RegisterComponent<FloorComponent>();

    TestObject* tmp = new TestObject();
<<<<<<< Updated upstream:src/InGame/Scenes/TestScene.cpp
    FloorObject* floor = new FloorObject();
//    TestObject* tmp1 = new TestObject();
//    TestObject* tmp2 = new TestObject();

    tmp->Init();
    floor->Init();
//    tmp1->Init();
//    tmp2->Init();
=======
    TestObject* tmp1 = new TestObject();
    TestUIObject* uiObj = new TestUIObject();

    tmp->Init();
    tmp1->Init();
    uiObj->Init();
>>>>>>> Stashed changes:src/InGame/Scenes/Test/TestScene.cpp

    tmp1->transform->SetLocalPos(Vec2(500, 500));

}
