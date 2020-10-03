//
// Created by EunwooSong on 2020-09-28.
//

#include "../../../SceneManager.h"
#include "../../../Scene.h"
#include "../../../GameObject.h"

using namespace SharkEngine::Core;

void SceneManager::Update() {
    const std::map<std::string, GameObject*> tmp = *currentScene->GetRootGameObjects();

    for(auto iter : tmp) {
        iter.second->Update();
    }
}

void SceneManager::LateUpdate() {
    const std::map<std::string, GameObject*> tmp = *currentScene->GetRootGameObjects();

    for(auto iter : tmp) {
        iter.second->LateUpdate();
    }
}

void SceneManager::Render() {
    const std::map<std::string, GameObject*> tmp = *currentScene->GetRootGameObjects();

    for(auto iter : tmp) {
        iter.second->Render();
    }
}

void SceneManager::EndScene() {
    const std::map<std::string, GameObject*> tmp = *currentScene->GetRootGameObjects();

    for(auto iter : tmp) {
        iter.second->EndScene();
    }

    currentScene->EndScene();
}

void SceneManager::ChangeScene(Scene *iter) {
    currentScene = iter;
}

SceneManager *SceneManager::Instance() {
    static SceneManager* iter = new SceneManager();
    return iter;
}
