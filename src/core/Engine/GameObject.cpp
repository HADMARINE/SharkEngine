//
// Created by EunwooSong on 2020-09-27.
//

#include "../../GameObject.h"
#include "../../Component.h"
#include "../../SceneManager.h"
#include "../../Scene.h"

SharkEngine::Core::GameObject::GameObject(std::string _ID) {
    Awake();
}

SharkEngine::Core::GameObject::~GameObject() {
}

void SharkEngine::Core::GameObject::Awake() {
}

void SharkEngine::Core::GameObject::Update() {
    for(std::map<std::string, SharkEngine::Core::Component*>::iterator iter = m_Components.begin(); iter != m_Components.end(); iter++)
        iter->second->Update();
    for(std::map<std::string, SharkEngine::Core::GameObject*>::iterator iter = m_Children.begin(); iter != m_Children.end(); iter++)
        iter->second->Update();
}

void SharkEngine::Core::GameObject::LateUpdate() {
    for(std::map<std::string, SharkEngine::Core::Component*>::iterator iter = m_Components.begin(); iter != m_Components.end(); iter++)
        iter->second->LateUpdate();
    for(std::map<std::string, SharkEngine::Core::GameObject*>::iterator iter = m_Children.begin(); iter != m_Children.end(); iter++)
        iter->second->LateUpdate();
}
void SharkEngine::Core::GameObject::Render() {
    for(std::map<std::string, SharkEngine::Core::Component*>::iterator iter = m_Components.begin(); iter != m_Components.end(); iter++)
        iter->second->Render();
    for(std::map<std::string, SharkEngine::Core::GameObject*>::iterator iter = m_Children.begin(); iter != m_Children.end(); iter++)
        iter->second->Render();
}

void SharkEngine::Core::GameObject::AddChild(SharkEngine::Core::GameObject * iter) {
    m_Children.insert(std::pair<std::string, SharkEngine::Core::GameObject*>(iter->GetName(), iter));
}

void SharkEngine::Core::GameObject::SetParent(SharkEngine::Core::GameObject * iter) {
    m_pParent = iter;
}

void SharkEngine::Core::GameObject::AddComponent(SharkEngine::Core::Component* iter) {
    m_Components.insert(std::pair<std::string, SharkEngine::Core::Component*>(iter->GetComponentID(), iter));
}

template<typename T>
T *SharkEngine::Core::GameObject::GetComponent() {
    for(auto i : m_Components) {
        T* tmp = dynamite_cast<T*>(i);
        if(tmp != nullptr)
            return tmp;
    }

    return nullptr;
}
void SharkEngine::Core::GameObject::OnMessage(SharkEngine::Core::Message * mes) {
    for(auto iter : m_Components) {
        iter.second->OnMessage(mes);
    }
}

void SharkEngine::Core::GameObject::Destroy(SharkEngine::Core::GameObject * iter) {
    iter->Destroy();
}
void SharkEngine::Core::GameObject::EndScene() {
}
