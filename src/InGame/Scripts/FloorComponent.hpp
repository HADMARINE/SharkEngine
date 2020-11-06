//
// Created by joonl on 2020-11-06.
//

#ifndef VULKAN_ENGINE_FLOORCOMPONENT_HPP
#define VULKAN_ENGINE_FLOORCOMPONENT_HPP

#include "../../CLogger.hpp"
#include "../../include/Engine/Scene/Components/Base/Component.h"
#include "include/Engine/Scene/GameObject/GameObject.h"
#include "include/Engine/Scene/Components/Transform.h"
#include "include/Engine/Scene/Components/Sprite2DRenderer.h"

class FloorComponent : public SharkEngine::Core::Component {
public:
    FloorComponent(){};
    ~FloorComponent(){};

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


#endif //VULKAN_ENGINE_FLOORCOMPONENT_HPP
