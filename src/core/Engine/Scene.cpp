//
// Created by EunwooSong on 2020-09-28.
//

#include "../../Scene.h"
#include "../../GameObject.h"
using namespace SharkEngine::Core;

const std::map<std::string, GameObject *>* SharkEngine::Core::Scene::GetRootGameObjects() {
    return &sceneObject;
}
GameObject *Scene::FindGameObject(std::string szName) {
    return sceneObject[szName];
}
Scene::Scene() {
}
Scene::~Scene() {
}
void Scene::EndScene() {
}
void Scene::AddGameObject(GameObject * iter) {
    sceneObject.insert(std::pair<std::string, GameObject*>(iter->GetName(), iter));
}
