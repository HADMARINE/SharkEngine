//
// Created by SongEunwoo on 2020/10/12.
//

#include "../../../../include/Engine/Scene/Manager/ComponentManager.h"
#include "../../../../include/Engine/CoreDefine.h"
#include "../../../../include/Engine/Scene/Components/Base/Component.h"

using namespace SharkEngine::Core;

void ComponentManager::Start()
{
    for(auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        for (auto compoIter : *components->GetComponentArray()) {
            if (compoIter)
                if (!compoIter->GetIsDestroy())
                    compoIter->Start();
        }
    }
}
void ComponentManager::Update() {
    for(auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : *componentArray) {
            if(compoIter)
                compoIter->Update();
        }
    }
}
void ComponentManager::LateUpdate() {
    for(auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : *componentArray) {
            if (compoIter)
                compoIter->LateUpdate();
        }
    }
}
void ComponentManager::Render() {
    for(auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : *componentArray) {
            if (compoIter)
                compoIter->Render();
        }
    }
}
void ComponentManager::EndScene() {
    for(auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : *componentArray) {
            if (compoIter)
                compoIter->EndScene();
        }
    }
}
ComponentManager::~ComponentManager() {
    for(auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for(auto compoIter : *componentArray) {
            SAFE_DELETE(compoIter);
        }
    }
}
