//
// Created by EunwooSong on 2020-11-01.
//

#ifndef SHARKENGINE_TESTCOMPONENT_H
#define SHARKENGINE_TESTCOMPONENT_H

#include "../../CLogger.hpp"
#include "../../include/Engine/Scene/Components/Base/Component.h"
#include "include/Engine/Scene/GameObject/GameObject.h"
#include "include/Engine/SharkSystem.hpp"
#include "include/Engine/Scene/Components/Transform.h"
#include "include/Engine/Scene/Components/Sprite2DRenderer.h"

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

    virtual void Update() override {
        Component::Update();
        CLogger::Debug("%d Test Update", (int)m_Owner->GetEntityID());

        GetOwner()->transform->Translate(Vec2(20.0f, 20.0f) * SHARK_TIME_MGR->GetDeltaTime());

        SHARK_LINE_MGR->DrawBox(GetOwner()->GetComponent<SharkEngine::Core::Transform>()->GetWorldPos(),
                                GetOwner()->GetComponent<SharkEngine::Core::Transform>()->GetWorldPos() + GetOwner()->GetComponent<SharkEngine::Core::Sprite2DRenderer>()->GetTextureSize(),
                                D3DCOLOR_ARGB(255,255,0,0));
                                //GetOwner()->transform->GetMatrix());
    }
};

#endif//SHARKENGINE_TESTCOMPONENT_H
