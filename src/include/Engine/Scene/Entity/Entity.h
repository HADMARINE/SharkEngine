//
// Created by EunwooSong on 2020-10-26.
//

#pragma once

#include "../../../../include/Engine/SharkSystem.hpp"
#include "../../CoreTypes.h"
#include "../Components/Base/Component.h"
#include "../Scene.h"
#include <vector>

namespace SharkEngine::Core {
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
        void AddComponent(SharkEngine::Core::Component *iter) {
            m_Scene->AddComponent<T>(m_id, iter);
            iter->SetOwner(reinterpret_cast<EntityID *>(this));
        };

        template<typename T>
        void DestroyComponent() {
            SHARK_ENGINE->GetSceneManager()->GetCurrentScene()->DestroyComponent<T>(m_id);
        };

        template<typename T>
        T *GetComponent() {
            SHARK_ENGINE->GetSceneManager()->GetCurrentScene()->GetComponent<T>(m_id);
        };

    protected:
        EntityID m_id;
        Scene *m_Scene;
        std::vector<Component *> m_components;
        bool isDestroy;
        bool isActive;
    };
}// namespace SharkEngine::Core