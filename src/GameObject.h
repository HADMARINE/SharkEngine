//
// Created by EunwooSong on 2020-09-27.
//

#ifndef VULKAN_ENGINE_GAMEOBJECT_H
#define VULKAN_ENGINE_GAMEOBJECT_H

#pragma once
#include <map>
#include <list>
#include <string>


namespace SharkEngine::Core {
    class Message;
    class Component;
    class Scene;
    class GameObject;

    //----------------------------------------------------------------
    // GameObject (Component System)
    //----------------------------------------------------------------
    class GameObject {
    public:
        GameObject(std::string _ID);
        virtual ~GameObject();

        //Main Loop ------------
        virtual void Awake() = 0;       //Initialize GameObjects, Children
        void Update();
        void LateUpdate();
        void Render();
        void EndScene();

        //Control Children Object -----------
        void AddChild(GameObject*);
        void SetParent(GameObject*);
        GameObject* GetParent() {return m_pParent;}

        //Control Component --------
        void AddComponent(Component*);
        template <typename T>
        T* GetComponent(std::string _id = "");
        void ProcessMessage(Message*);
        void AddMessage(Message*);

        //Control MyState -------------
        void Destroy();
        bool IsDestroy() {return m_bIsDestroy; }
        std::string GetName() {return _ID;}
        static void Destroy(GameObject*);

    private:
        std::string _ID;
        std::map<std::string, Component *> m_Components;
        std::map<std::string, GameObject *> m_Children;
        std::list<Message*> messageQueue;

        GameObject * m_pParent;

        Scene * m_currentScene;

        bool m_bIsActive;
        bool m_bIsDestroy;

        int m_iLayer;
    };
}
#endif//VULKAN_ENGINE_GAMEOBJECT_H
