//
// Created by EunwooSong on 2020-10-25.
//

#ifndef VULKAN_ENGINE_COMPONENT_H
#define VULKAN_ENGINE_COMPONENT_H

#include "../../../CoreTypes.h"
#include "../../../CoreStructs.h"

namespace SharkEngine::Core {
    class ComponentManager;

    class Component {
    public:
        Component() : isDestroy(false), isActive(true), isStarted(false){};
        ~Component(){};
        virtual void Start()        {}
        virtual void Update()       {}
        virtual void LateUpdate()   {}
        virtual void Render()       {}
        virtual void EndScene()     {}

        friend ComponentManager;

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
        bool isDestroy;
        bool isActive;
        bool isStarted;

        //Component Listeners
    public:
        virtual void OnCollisionEnter(Collision*){};
        virtual void OnCollisionStay(Collision*){};
        virtual void OnCollisionExit(Collision*){};
        virtual void OnTriggerEnter(Collision*){};
        virtual void OnTriggerStay(Collision*){};
        virtual void OnTriggerExit(Collision*){};
    };
}


#endif//VULKAN_ENGINE_COMPONENT_H
