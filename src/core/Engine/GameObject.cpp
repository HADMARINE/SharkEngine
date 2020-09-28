//
// Created by EunwooSong on 2020-09-27.
//

#include "../../GameObject.h"
SharkEngine::Core::GameObject::GameObject(std::string _ID) {

}

SharkEngine::Core::GameObject::~GameObject() {

}

void SharkEngine::Core::GameObject::Awake() {

}

void SharkEngine::Core::GameObject::Start() {

}

void SharkEngine::Core::GameObject::Update() {

}

void SharkEngine::Core::GameObject::LateUpdate() {

}
void SharkEngine::Core::GameObject::Render() {

}

void SharkEngine::Core::GameObject::AddChild(SharkEngine::Core::GameObject *) {

}

void SharkEngine::Core::GameObject::Destroy(SharkEngine::Core::GameObject *) {

}

void SharkEngine::Core::GameObject::SetParent(SharkEngine::Core::GameObject *) {

}

void SharkEngine::Core::GameObject::AddComponent(SharkEngine::Core::Component *) {

}

template<typename T>
T *SharkEngine::Core::GameObject::GetComponent() {
    return nullptr;
}

SharkEngine::Core::GameObject *SharkEngine::Core::GameObject::Find(std::string szName) {
    return nullptr;
}
