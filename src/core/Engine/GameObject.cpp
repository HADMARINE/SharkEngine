//
// Created by EunwooSong on 2020-09-27.
//

#include "../../GameObject.h"
#include "../../Component.h"
#include "../../SceneManager.h"
#include "../../Scene.h"
#include "../../CoreDefine.h"

SharkEngine::Core::GameObject::GameObject(std::string _ID) : _ID(_ID) {
    m_currentScene = SceneMGR->currentScene;
    m_currentScene->AddGameObject(this);

    Awake();
}

SharkEngine::Core::GameObject::~GameObject() {
    for(auto iter : m_Components)
        SAFE_DELETE(iter.second);

    for(auto iter : messageQueue)
        SAFE_DELETE(iter);
}

// -------------------------------------------------------------------------
// Called when this Object(GameObject) is Created
void SharkEngine::Core::GameObject::Awake() {
}

// -------------------------------------------------------------------------
// Called when Current Process State is Update
void SharkEngine::Core::GameObject::Update() {
    for(std::map<std::string, SharkEngine::Core::Component*>::iterator iter = m_Components.begin(); iter != m_Components.end(); iter++)
        iter->second->Update();
}

// -------------------------------------------------------------------------
// Called when Current Process State is LateUpdate
void SharkEngine::Core::GameObject::LateUpdate() {
    for(std::map<std::string, SharkEngine::Core::Component*>::iterator iter = m_Components.begin(); iter != m_Components.end(); iter++)
        iter->second->LateUpdate();
}

// -------------------------------------------------------------------------
// Called when Current Process State is Render
void SharkEngine::Core::GameObject::Render() {
    for(std::map<std::string, SharkEngine::Core::Component*>::iterator iter = m_Components.begin(); iter != m_Components.end(); iter++)
        iter->second->Render();
}

// -------------------------------------------------------------------------
// Add GameObjects m_Children
void SharkEngine::Core::GameObject::AddChild(SharkEngine::Core::GameObject * iter) {
    m_Children.insert(std::pair<std::string, SharkEngine::Core::GameObject*>(iter->GetName(), iter));
    iter->SetParent(this);
}

// -------------------------------------------------------------------------
// Set GameObject's m_pParent
// * Top Level GameObject's m_pParent is nullPtr
void SharkEngine::Core::GameObject::SetParent(SharkEngine::Core::GameObject * iter) {
    m_pParent = iter;
}

// -------------------------------------------------------------------------
// Add Component to this GameObject
void SharkEngine::Core::GameObject::AddComponent(SharkEngine::Core::Component* iter) {
    m_Components.insert(std::pair<std::string, SharkEngine::Core::Component*>(iter->GetComponentID(), iter));
}

// -------------------------------------------------------------------------
// Find Component With Type or _ID
template<typename T>
T *SharkEngine::Core::GameObject::GetComponent(std::string _id) {
    if(_id.Length() > 0)
        if(m_Components[_id])
            return m_Components[_id];
        else
            return nullptr;

    for(auto i : m_Components) {
        T* tmp = dynamite_cast<T*>(i);
        if(tmp != nullptr)
            return tmp;
    }

    return nullptr;
}

// -------------------------------------------------------------------------
// Process Message in MessageQueue
void SharkEngine::Core::GameObject::ProcessMessage(SharkEngine::Core::Message * mes) {
    for(auto iter : m_Components) {
        iter.second->OnMessage(mes);
    }
}

// - Static ----------------------------------------------------------------
// Destroy GameObject when EndScene is Called
void SharkEngine::Core::GameObject::Destroy(SharkEngine::Core::GameObject * iter) {
    iter->Destroy();
}

// -------------------------------------------------------------------------
// Called when Scene is End
void SharkEngine::Core::GameObject::EndScene() {
    m_Children.erase(remove_if(m_Children.begin(), m_Children.end(),
        [&](auto iter) {
        return iter->second->IsDestroy();
    }), m_Children.end());

    for(std::map<std::string, SharkEngine::Core::Component*>::iterator iter = m_Components.begin(); iter != m_Components.end(); iter++)
        iter->second->EndScene();

    for(Message* mes : messageQueue) {
        ProcessMessage(mes);
    }
    for(Message* mes : messageQueue) {
        SAFE_DELETE(mes);
    }
    messageQueue.clear();
}

// -------------------------------------------------------------------------
// Called when My Component Called SendMessage
void SharkEngine::Core::GameObject::AddMessage(Message* mes) {
    messageQueue.push_back(mes);
}

// -------------------------------------------------------------------------
// Destroy this GameObject And its Children
void SharkEngine::Core::GameObject::Destroy() {
    m_bIsDestroy = true;

    for(auto iter : m_Children)
        Destroy(iter.second);
}