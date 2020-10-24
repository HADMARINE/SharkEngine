//
// Created by EunwooSong on 2020-10-23.
//

#include "../../../../include/Engine/Scene/Manager/SignatureManager.h"
#include "../../../../include/Engine/Scene/Scene.h"

using namespace SharkEngine::Core;

template<typename T>
shared_ptr<T> SignatureManager::RegisterSystem() {
    const char* typeName = typeid(T).name();

    // Create a pointer to the system and return it so it can be used externally
    auto system = std::make_shared<T>();
    m_Systems.insert({typeName, system});
    return system;
}

template<typename T>
void SignatureManager::SetSignature(Signature signature) {
    const char* typeName = typeid(T).name();

    // Set Signature
    m_Signatures.insert({typeName, signature});

}

void SignatureManager::EntityDestroyed(EntityID _ID) {
    for(auto const& pair : m_Scenes) {
        auto const& scene = pair.second;

        scene->m_EntityList.erase(_ID);
    }
}

void SignatureManager::EntitySignatureChanged(EntityID _ID, Signature _Signature) {
    for(auto const& pair : m_Scenes) {
        auto const& type = pair.first;
        auto const& scene = pair.second;
        auto const& sceneSignature = m_Signatures[type];

        // Entity signature matches system signature - insert into set
        if ((_Signature & sceneSignature) == sceneSignature)
        {
            scene->m_EntityList.insert(_ID);
        }
            // Entity signature does not match system signature - erase from set
        else
        {
            scene->m_EntityList.erase(_ID);
        }
    }
}