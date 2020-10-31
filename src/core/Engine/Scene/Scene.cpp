//
// Created by EunwooSong on 2020-10-23.
//

#include "../../../include/Engine/Scene/Scene.h"
#include "../../../include/Engine/Scene/Manager/ComponentManager.h"
#include "../../../include/Engine/Scene/Manager/EntityIDManager.h"
#include "../../../include/Engine/Scene/Manager/SignatureManager.h"

SharkEngine::Core::Scene::Scene() {
}
SharkEngine::Core::Scene::~Scene() {
    //Reset Signature
}
void SharkEngine::Core::Scene::Init() {
    m_ComponentManager = make_unique<ComponentManager>();
    m_EntityIDManager = make_unique<EntityIDManager>();
    m_SignatureManager = make_unique<SignatureManager>();

    //Register Component(Transform, RigidBody, 2D Renderer, Collider . . .)
}
void Scene::Start() {
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
SharkEngine::Core::EntityID SharkEngine::Core::Scene::CreateEntity() {
    return m_EntityIDManager->CreateEntityID();
}
void SharkEngine::Core::Scene::DestroyEntity(EntityID _id) {
    m_EntityIDManager->DestroyEntityID(_id);
    m_ComponentManager->EntityDestroyed(_id);
    m_SignatureManager->EntityDestroyed(_id);
}

template<typename T>
void SharkEngine::Core::Scene::RegisterComponent() {
    m_ComponentManager->RegisterComponent<T>();
}

template<typename T>
void SharkEngine::Core::Scene::AddComponent(EntityID _id, Component* _compo) {
    m_ComponentManager->AddComponent<T>(_id, _compo);

    auto signature = m_EntityIDManager->GetSignature(_id);
    signature.set(m_ComponentManager->GetComponentID<T>(), true);
    m_EntityIDManager->SetSignature(_id, signature);

    m_SignatureManager->EntitySignatureChanged(_id, signature);
}

template<typename T>
void SharkEngine::Core::Scene::RemoveComponent(SharkEngine::Core::EntityID _id) {
    m_ComponentManager->DestroyComponent<T>(_id);

    auto signature = m_EntityIDManager->GetSignature(_id);
    signature.set(m_ComponentManager->GetComponentID<T>(), false);
}

template<typename T>
T *SharkEngine::Core::Scene::GetComponent(SharkEngine::Core::EntityID _id) {
    return m_ComponentManager->GetComponent<T>(_id);
}

template<typename T>
SharkEngine::Core::ComponentID SharkEngine::Core::Scene::GetComponentType() {
    return m_ComponentManager->GetComponentID<T>();
}

template<typename T>
shared_ptr<T> SharkEngine::Core::Scene::RegisterSystem() {
    return m_SignatureManager->RegisterScene<T>();
}

template<typename T>
void SharkEngine::Core::Scene::SetSystemSignature(SharkEngine::Core::Signature signature) {
    m_SignatureManager->SetSignature<T>(signature);
}