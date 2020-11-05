//
// Created by EunwooSong on 2020-11-01.
//

#include "TestObject.h"
#include "../Scripts/TestComponent.h"
#include "../../include/Engine/Scene/Scene.h"
#include "../../CLogger.hpp"
using namespace SharkEngine::Core;

void TestObject::Init() {
    GameObject::Init();

    CLogger::Info("Test Object Initialize");

    testComponent = new TestComponent();
    m_Scene->AddComponent<TestComponent>(this, testComponent);

    //if(m_Scene->GetComponent<Transform>(0)->GetOwner() == this)
    if(transform)
        CLogger::Error("Transform Test");
};
