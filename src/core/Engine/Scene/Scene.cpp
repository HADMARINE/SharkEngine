//
// Created by EunwooSong on 2020-10-23.
//

#include "../../../include/Engine/Scene/Scene.h"

SharkEngine::Core::Scene::Scene() {
}
SharkEngine::Core::Scene::~Scene() {
    //Reset Signature
}
void SharkEngine::Core::Scene::Init() {
    m_ComponentManager = std::make_unique<ComponentManager>();
    m_EntityIDManager = std::make_unique<EntityIDManager>();
    m_SignatureManager = std::make_unique<SignatureManager>();

    //Register Component(Transform, RigidBody, 2D Renderer, Collider . . .)
}
void SharkEngine::Core::Scene::Start() {
    m_ComponentManager->Start();
}
void SharkEngine::Core::Scene::Update() {
    m_ComponentManager->Update();
}
void SharkEngine::Core::Scene::LateUpdate() {
    m_ComponentManager->LateUpdate();
}
void SharkEngine::Core::Scene::Render() {
    m_ComponentManager->Render();
}
void SharkEngine::Core::Scene::EndScene() {
    //Destroy Entity Objects
    m_ComponentManager->EndScene();
}
