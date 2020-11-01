//
// Created by EunwooSong on 2020-10-26.
//

#ifndef VULKAN_ENGINE_ENTITY_H
#define VULKAN_ENGINE_ENTITY_H

#include <vector>
#include "../../CoreTypes.h"
#include "../Components/Base/Component.h"
#include "../Scene.h"

namespace SharkEngine::Core {
    class Component;
    class Scene;

    class Entity {
    public:
        Entity();
        ~Entity();

        virtual void Init() = 0;

        bool GetIsDestroy();
        void SetIsDestroy(bool);

        bool GetIsActive();
        void SetIsActive(bool);

        EntityID GetEntityID();

        template<typename T>
        void AddComponent(SharkEngine::Core::Component* iter) {
            m_Scene->AddComponent<T>(m_id, iter);
            iter->SetOwner(this);
        };

        template<typename T>
        void DestroyComponent();

        template<typename T>
        T* GetComponent();

    protected:
        EntityID m_id;
        Scene* m_Scene;
        std::vector<Component*> m_components;
        bool isDestroy;
        bool isActive;
    };
}

#endif//VULKAN_ENGINE_ENTITY_H
