//
// Created by EunwooSong on 2020-10-26.
//

#include "../../../../include/Engine/Scene/Entity/Entity.h"
#include "../../../../include/Engine/SystemManager/SceneManager.h"
#include "../../../../include/Engine/Scene/Scene.h"
#include "../../../../include/Engine/SharkSystem.hpp"

SharkEngine::Core::Entity::Entity() {
    m_Scene = SHARK_ENGINE->GetSceneManager()->GetCurrentScene();
    m_id = m_Scene->CreateEntity(this);
}
SharkEngine::Core::Entity::~Entity() {
}
bool SharkEngine::Core::Entity::GetIsDestroy() {
    return isDestroy;
}
void SharkEngine::Core::Entity::SetIsDestroy(bool isDestroy) {
    this->isDestroy = isDestroy;
}
bool SharkEngine::Core::Entity::GetIsActive() {
    return isActive;
}
void SharkEngine::Core::Entity::SetIsActive(bool isActive) {
    this->isActive = isActive;
}
SharkEngine::Core::EntityID SharkEngine::Core::Entity::GetEntityID() {
    return m_id;
}
//template<typename T>
//void SharkEngine::Core::Entity::DestroyComponent() {
//    SHARK_ENGINE->GetSceneManager()->GetCurrentScene()->DestroyComponent<T>(m_id);
//}
//template<typename T>
//T *SharkEngine::Core::Entity::GetComponent() {
//    SHARK_ENGINE->GetSceneManager()->GetCurrentScene()->GetComponent<T>(m_id);
//}
