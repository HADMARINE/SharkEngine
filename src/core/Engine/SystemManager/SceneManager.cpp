//
// Created by EunwooSong on 2020-10-13.
//

#include "../../../include/Engine/SystemManager/SceneManager.h"
#include "../../../include/Engine/Scene/Scene.h"
#include "../../../include/Graphic/IncludeVulkan.h"

using namespace SharkEngine::Core;
void SceneManager::Update() {
    currentScene->Update();
}
void SceneManager::LateUpdate() {
    currentScene->LateUpdate();
}
void SceneManager::Render() {
    currentScene->Render();
}
void SceneManager::EndScene() {
    currentScene->EndScene();
}
const Scene *SceneManager::GetCurrentScene() {
    return currentScene;
}
template<typename T>
void SceneManager::ChangeScene() {
    T* currentScene = new T();

    SAFE_DELETE(this->currentScene);
    this->currentScene = dynamic_cast<Scene*>(currentScene);
}
void SceneManager::MainLoop() {
    Update();
    LateUpdate();
    Render();
    EndScene();
}
