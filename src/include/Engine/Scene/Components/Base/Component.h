//
// Created by EunwooSong on 2020-10-25.
//

#pragma once

#include "../../../CoreStructs.h"
#include "../../../CoreTypes.h"
#include "../../Entity/Entity.h"

namespace SharkEngine::Core {
    class ComponentManager;
    class Collision;

    class Component {
    public:
        Component() : isDestroy(false), isActive(true), isStarted(false){};
        ~Component(){};
        virtual void Start() { isStarted = true; }
        virtual void Update() {}
        virtual void LateUpdate() {}
        virtual void Render() {}
        virtual void EndScene() {}

        void Destroy() { isDestroy = true; }
        static void Destroy(Component *compo) { compo->Destroy(); }
        bool GetIsDestroy() const { return isDestroy; }

        void SetActive(bool isActive) { this->isActive = isActive; }
        bool GetActive() const { return isActive; }

        Entity *GetOwner() const { return m_Owner; }
        void SetOwner(Entity *owner) { m_Owner = owner; }

        bool GetIsStarted() { return isStarted; }

    protected:
        Entity *m_Owner;
        bool isDestroy;
        bool isActive;
        bool isStarted;

        //Component Listeners
    public:
        virtual void OnCollisionEnter(Collision *){};
        virtual void OnCollisionStay(Collision *){};
        virtual void OnCollisionExit(Collision *){};
        virtual void OnTriggerEnter(Collision *){};
        virtual void OnTriggerStay(Collision *){};
        virtual void OnTriggerExit(Collision *){};
    };
}// namespace SharkEngine::Core