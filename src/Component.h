//
// Created by EunwooSong on 2020-09-28.
//

#ifndef VULKAN_ENGINE_COMPONENT_H
#define VULKAN_ENGINE_COMPONENT_H

#include <string>

class GameObject;

namespace SharkEngine::Core {
    class Component {
    public:
        Component(std::string _ID);
        virtual ~Component(void);

        const std::string GetComponentID() const;

        void SetOwner(GameObject* obj);
        GameObject* GetOwner() const;

        virtual void Awake(void);
        virtual void Update(void);
        virtual void Render(void);

        virtual void OnMessage();
        virtual void SendMessage();

    protected:
        GameObject* m_pOwner;
        std::string _id;
    };
}



#endif//VULKAN_ENGINE_COMPONENT_H
