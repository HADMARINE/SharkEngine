//
// Created by EunwooSong on 2020-10-13.
//

#include "../../../include/Engine/SystemManager/SceneManager.h"
#include "../../../include/Engine/Scene/Scene.h"
#include "../../../stdafx.hpp"
#include "../../../CLogger.hpp"

using namespace SharkEngine::Core;
void SceneManager::Start() {
    if(currentScene) currentScene->Start();
}
void SceneManager::Update() {
    if(currentScene) currentScene->Update();
}
void SceneManager::LateUpdate() {
    if(currentScene) currentScene->LateUpdate();
}
void SceneManager::Render() {
    if(currentScene) currentScene->Render();
}
void SceneManager::EndScene() {
    if(currentScene) currentScene->EndScene();

    if(nextScene) {
        SAFE_DELETE(currentScene);
        currentScene = nextScene;
        nextScene = nullptr;

        currentScene->Init();
    }
}
Scene *SceneManager::GetCurrentScene() {
    return currentScene;
}
