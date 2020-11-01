//
// Created by EunwooSong on 2020-11-01.
//

#ifndef SHARKENGINE_TESTCOMPONENT_H
#define SHARKENGINE_TESTCOMPONENT_H

#include "../../CLogger.hpp"
#include "../../include/Engine/Scene/Components/Base/Component.h"

class TestComponent : public SharkEngine::Core::Component {
public:
    TestComponent(){};
    ~TestComponent(){};

    virtual void Start() override {
        Component::Start();
        CLogger::Debug("%d Test Start", (int)m_Owner->GetEntityID());
    }

    virtual void Update() override {
        Component::Update();
        CLogger::Debug("%d Test Update", (int)m_Owner->GetEntityID());
    }
};

#endif//SHARKENGINE_TESTCOMPONENT_H
