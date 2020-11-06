//
// Created by EunwooSong on 2020-10-23.
//

#include "../../../include/Engine/Scene/Scene.h"
#include "include/Engine/Scene/Components/Transform.h"
#include "include/Engine/Scene/Components/Sprite2DRenderer.h"
#include "include/Engine/Scene/Components/RigidBody2D.hpp"
#include "include/Engine/Scene/Components/BoxCollider.hpp"
#include "include/Engine/Scene/Components/Button.h"
#include "include/Engine/Scene/Components/CameraComponent.h"

SharkEngine::Core::Scene::Scene() {
}
SharkEngine::Core::Scene::~Scene() {
    //Reset Signature
    for(auto iter : m_EntityList) {
        SAFE_DELETE(iter.second);
    }
}
void SharkEngine::Core::Scene::Init() {
    m_ComponentManager = std::make_unique<ComponentManager>();
    m_EntityIDManager = std::make_unique<EntityIDManager>();
    m_ColliderManager = std::make_unique<ColliderManager>();

    //Register Component(Transform, RigidBody, 2D Renderer, Collider . . .)
    RegisterComponent<Transform>();
    RegisterComponent<CameraComponent>();
    RegisterComponent<RigidBody2D>();
    RegisterComponent<Sprite2DRenderer>();
    RegisterComponent<BoxCollider>();
    RegisterComponent<Button>();
}
void SharkEngine::Core::Scene::Start() {
    m_ComponentManager->Start();
}
void SharkEngine::Core::Scene::Update() {
    m_ComponentManager->Update();
    m_ColliderManager->Update();
}
void SharkEngine::Core::Scene::LateUpdate() {
    m_ComponentManager->LateUpdate();
    m_ColliderManager->LateUpdate();
}
void SharkEngine::Core::Scene::Render() {
    m_ComponentManager->Render();
}
void SharkEngine::Core::Scene::EndScene() {
    //Destroy Entity Objects
    m_ComponentManager->EndScene();

    for(auto iter = m_EntityList.begin(); iter != m_EntityList.end(); ++iter) {
        if(iter->second->GetIsDestroy()) {
            auto tmp = iter++;
            DestroyEntity(tmp->first);

            if(iter == m_EntityList.end())
                break;
        }
    }
}

std::vector<SharkEngine::Core::Component *> SharkEngine::Core::Scene::FindEntityComponents(SharkEngine::Core::EntityID _id) {
    return m_ComponentManager->FindEntityComponents(_id);
}
