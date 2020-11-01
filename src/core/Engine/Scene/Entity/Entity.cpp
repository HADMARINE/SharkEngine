//
// Created by EunwooSong on 2020-10-26.
//

#include "../../../../include/Engine/SharkSystem.hpp"

SharkEngine::Core::Entity::Entity() : m_id(-1), m_Scene(nullptr), isDestroy(false), isActive(true) {
    m_Scene = SHARK_ENGINE->GetSceneManager()->GetCurrentScene();
    m_id = m_Scene->CreateEntity(this);

    Init();
}
SharkEngine::Core::Entity::~Entity() {
}
void SharkEngine::Core::Entity::Init() {
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