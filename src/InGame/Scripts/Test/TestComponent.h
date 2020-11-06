//
// Created by EunwooSong on 2020-11-01.
//

#ifndef SHARKENGINE_TESTCOMPONENT_H
#define SHARKENGINE_TESTCOMPONENT_H

#include "CLogger.hpp"
#include "InGame/Scenes/Test/TestScene.h"
#include "include/Engine/Scene/Components/Base/Component.h"
#include "include/Engine/Scene/Components/Sprite2DRenderer.h"
#include "include/Engine/Scene/Components/Transform.h"
#include "include/Engine/Scene/GameObject/GameObject.h"
#include "include/Engine/SharkSystem.hpp"

class TestComponent : public SharkEngine::Core::Component {
public:
    TestComponent(){};
    ~TestComponent(){};

    virtual void Start() override {
        Component::Start();
        CLogger::Debug("%d Test Start", (int)m_Owner->GetEntityID());

        SharkEngine::Core::Transform* tmp = dynamic_cast<SharkEngine::Core::GameObject *>(m_Owner)->GetComponent<SharkEngine::Core::Transform>();

        if(tmp != nullptr)
            CLogger::Error("tmp");

        m_Owner->GetScene()->GetComponent<SharkEngine::Core::Transform>(m_Owner->GetEntityID());
    }

    void Update() override;
};

#endif//SHARKENGINE_TESTCOMPONENT_H
