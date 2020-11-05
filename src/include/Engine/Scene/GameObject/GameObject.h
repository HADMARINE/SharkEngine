//
// Created by EunwooSong on 2020-11-05.
//

#ifndef SHARKENGINE_GAMEOBJECT_H
#define SHARKENGINE_GAMEOBJECT_H

#include "../Entity/Entity.h"
#include "../Scene.h"
#include "../Components/Transform.h"

namespace SharkEngine::Core {
    class GameObject :
        public SharkEngine::Core::Entity {
    public:
        GameObject(){}
        ~GameObject(){}

        virtual void Init() override;

        template<typename T>
        T* AddComponent() {
            T* compo = new T();
            m_Scene->AddComponent<T>(this, compo);
            return compo;
        }

        template<typename T>
        T* GetComponent() {
            return m_Scene->GetComponent<T>(_id);
        }

    protected:
        Transform* transform;
    };

}

#endif//SHARKENGINE_GAMEOBJECT_H
