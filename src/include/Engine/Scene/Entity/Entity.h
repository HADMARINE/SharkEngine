//
// Created by EunwooSong on 2020-10-26.
//

#pragma once

#include "../../CoreTypes.h"
#include "../Scene.h"
#include <vector>

namespace SharkEngine::Core {
    class Component;
    class Scene;

    class Entity {
    public:
        Entity();
        ~Entity();

        virtual void Init();

        bool GetIsDestroy();
        void SetIsDestroy(bool);

        bool GetIsActive();
        void SetIsActive(bool);

        EntityID GetEntityID();

    protected:
        EntityID m_id;
        Scene *m_Scene;
        std::vector<Component *> m_components;
        bool isDestroy;
        bool isActive;
    };
}// namespace SharkEngine::Core