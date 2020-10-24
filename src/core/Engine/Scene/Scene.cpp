//
// Created by EunwooSong on 2020-10-23.
//

#include "../../../include/Engine/Scene/Scene.h"
#include "../../../include/Engine/Scene/Manager/ComponentManager.h"
#include "../../../include/Engine/Scene/Manager/EntityIDManager.h"
#include "../../../include/Engine/Scene/Manager/SignatureManager.h"

void SharkEngine::Core::Scene::Init() {
}
void SharkEngine::Core::Scene::Update() {
    m_ComponentManager->Update();
}
void SharkEngine::Core::Scene::LateUpdate() {
}
void SharkEngine::Core::Scene::Render() {
}
void SharkEngine::Core::Scene::EndScene() {
}
SharkEngine::Core::Scene::Scene() {
}
SharkEngine::Core::Scene::~Scene() {
}
