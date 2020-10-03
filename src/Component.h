//
// Created by EunwooSong on 2020-09-28.
//

#ifndef VULKAN_ENGINE_COMPONENT_H
#define VULKAN_ENGINE_COMPONENT_H

#include <string>

namespace SharkEngine::Core {
    class GameObject;
    class Message;

    class Component {
    public:
        Component(std::string _ID);
        virtual ~Component(void);

        const std::string GetComponentID() const;

        void SetOwner(GameObject* obj);
        GameObject* GetOwner() const;

        virtual void Awake(void) = 0;
        virtual void Update(void) = 0;
        virtual void Render(void) = 0;
        virtual void LateUpdate(void) = 0;
        virtual void EndScene(void) = 0;

        virtual void OnMessage(Message*) = 0;
        virtual void SendMessage() = 0;

    protected:
        GameObject* m_pOwner;
        std::string _id;
    };
}



#endif//VULKAN_ENGINE_COMPONENT_H
