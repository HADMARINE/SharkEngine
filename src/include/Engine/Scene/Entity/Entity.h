//
// Created by EunwooSong on 2020-10-26.
//

#ifndef VULKAN_ENGINE_ENTITY_H
#define VULKAN_ENGINE_ENTITY_H
#include "../../CoreTypes.h"

namespace SharkEngine::Core {
    class Entity {
    public:
        Entity();
        ~Entity();

        bool GetIsDestroy();
        void SetIsDestroy(bool);

        bool GetIsActive();
        void SetIsActive(bool);

        EntityID GetEntityID();
        void AddComponent();
    };
}

#endif//VULKAN_ENGINE_ENTITY_H
