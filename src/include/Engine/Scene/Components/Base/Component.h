//
// Created by EunwooSong on 2020-10-25.
//

#ifndef VULKAN_ENGINE_COMPONENT_H
#define VULKAN_ENGINE_COMPONENT_H

#include "../../CoreTypes.h"

namespace SharkEngine::Core {
    class Component {
    public:
        Component(){};
        ~Component(){};
        virtual void Update(){}
        virtual void LateUpdate(){}
        virtual void Render(){}
        virtual void EndScene(){}

        void Destroy() { isDestroy = true; }
        static void Destroy(Component* compo) { compo->Destroy(); }
        bool GetIsDestroy() const { return isDestroy; }

        ComponentID GetComponentID() { return m_ComponentID; }
        EntityID GetEntityID() const { return m_EntityID; }
  
        void SetActive(bool isActive) {this->isActive = isActive;}
        bool GetActive() const {return isActive;}

    protected:
        EntityID m_EntityID;
        ComponentID m_ComponentID;
        bool isDestroy = false;
        bool isActive = true;
    };
}


#endif//VULKAN_ENGINE_COMPONENT_H
