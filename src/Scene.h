//
// Created by EunwooSong on 2020-09-28.
//

#ifndef VULKAN_ENGINE_SCENE_H
#define VULKAN_ENGINE_SCENE_H

#include <list>
#include <map>
#include <string>

namespace SharkEngine::Core {
    class GameObject;

    class Scene {
    public:
        Scene();
        ~Scene();

        const std::map<std::string, GameObject*>* GetRootGameObjects();
        GameObject* FindGameObject(std::string szName);

        void AddGameObject(GameObject*);
        void EndScene();

        int handle;
        bool isLoaded;
        std::map<std::string, GameObject*> sceneObject;
    };
}

#endif//VULKAN_ENGINE_SCENE_H
