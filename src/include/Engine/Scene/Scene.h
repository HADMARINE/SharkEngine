//
// Created by EunwooSong on 2020-10-23.
//

#ifndef VULKAN_ENGINE_SCENE_H
#define VULKAN_ENGINE_SCENE_H

#include <iostream>
#include <list>
#include <map>
#include "../CoreTypes.h"

using namespace std;

namespace SharkEngine::Core {
    class Entity;
    class EntityIDManager;
    class ComponentManager;
    class SignatureManager;

    class Scene {
    public:
        Scene();
        ~Scene();

        virtual void Init();
        void Update();
        void LateUpdate();
        void Render();
        void EndScene();

        list<EntityID> m_EntityList;

    private:
        unique_ptr<ComponentManager> m_ComponentManager;
        unique_ptr<EntityIDManager> m_EntityManager;
        unique_ptr<SignatureManager> m_SystemManager;
    };
}

#endif//VULKAN_ENGINE_SCENE_H
