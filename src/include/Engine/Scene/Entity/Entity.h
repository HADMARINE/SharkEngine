//
// Created by EunwooSong on 2020-10-23.
//

#ifndef VULKAN_ENGINE_ENTITY_H
#define VULKAN_ENGINE_ENTITY_H

#include "../../CoreTypes.h"

namespace SharkEngine::Core {
    class Scene;

    class Entity {
    public:
        Entity();
        virtual ~Entity();

        void AddComponent(ComponentID componentID);
        template <typename T>
        T* GetComponent();
        void SetActive(bool active);
        bool GetActive();

        EntityID m_id;

        bool isDestroy;
        bool isActive;

        Scene* currentScene;
    };
}

#endif//VULKAN_ENGINE_ENTITY_H
