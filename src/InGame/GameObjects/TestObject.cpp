//
// Created by EunwooSong on 2020-11-01.
//

#include "TestObject.h"
#include "../Scripts/TestComponent.h"
#include "../../include/Engine/Scene/Scene.h"

using namespace SharkEngine::Core;

void TestObject::Init() {
    testComponent = new TestComponent();
    m_Scene->AddComponent<TestComponent>(this, testComponent);
};
