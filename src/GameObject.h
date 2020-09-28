//
// Created by EunwooSong on 2020-09-27.
//

#ifndef VULKAN_ENGINE_GAMEOBJECT_H
#define VULKAN_ENGINE_GAMEOBJECT_H

#pragma once
#include "stdafx.hpp"

namespace SharkEngine::Core {

    class Component;
    class Scene;
//----------------------------------------------------------------
// GameObject (Component System)
//----------------------------------------------------------------
    class GameObject {
    public:
        GameObject(std::string _ID);
        virtual ~GameObject();

        //Main Loop ------------
        void Awake();
        void Start();
        void Update();
        void LateUpdate();
        void Render();

        //Control Children Object -----------
        void AddChild(GameObject*);
        void Destroy(GameObject*);
        void SetParent(GameObject*);
        GameObject* GetParent() {return m_pParent;}

        //Control Component --------
        void AddComponent(Component*);
        template <typename T>
        T* GetComponent();

        //Control MyState -------------
        void Destroy() {m_bIsDestroy = true;}
        bool IsDestroy() {return m_bIsDestroy; }
        std::string & GetName() {return _ID;}

        //Control Others -------------
        static GameObject* Find(std::string szName);

    private:
        std::string _ID;
        std::map<std::string, Component *> m_Components;
        std::map<std::string, GameObject *> m_Children;
        GameObject * m_pParent;

        bool m_bIsActive;
        bool m_bIsDestroy;

        int m_iLayer;
    };

}
#endif//VULKAN_ENGINE_GAMEOBJECT_H
