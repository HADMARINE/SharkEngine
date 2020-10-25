//
// Created by SongEunwoo on 2020/10/12.
//

#ifndef VULKAN_ENGINE_COMPONENTMANAGER_H
#define VULKAN_ENGINE_COMPONENTMANAGER_H

#include "../../ComponentArray.hpp"
#include "../../CoreDefine.h"
#include "../../CoreTypes.h"
#include <unordered_map>
#include <shared_mutex>

using namespace std;

namespace SharkEngine::Core {
    class ComponentManager {
    public:
        template<typename T>
        void RegisterComponent();

        template<typename T>
        ComponentID GetComponentID();

        template<typename T>
        void AddComponent(EntityID, T*);

        template<typename T>
        T* GetComponent(EntityID);

        template<typename T>
        void DestroyComponent(EntityID);

        void EntityDestroyed(EntityID);

        void Update();
        void LateUpdate();
        void Render();
        void EndScene();

    private:
        unordered_map<const char*, ComponentID>m_ComponentIDs{};
        unordered_map<const char*, shared_ptr<IComponentArray>> m_ComponentArrays{};

        ComponentID m_nextComponentID;

        template<typename T>
        shared_ptr<ComponentArray<T>> GetComponentArray();

    };
}

#endif//VULKAN_ENGINE_COMPONENTMANAGER_H
