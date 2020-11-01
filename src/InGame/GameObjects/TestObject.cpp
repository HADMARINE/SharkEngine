//
// Created by EunwooSong on 2020-11-01.
//

#include "TestObject.h"
#include "../Scripts/TestComponent.h"

using namespace SharkEngine::Core;

void TestObject::Init() {
    testComponent = new TestComponent();
    AddComponent<TestComponent>(testComponent);
};
