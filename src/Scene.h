//
// Created by EunwooSong on 2020-09-28.
//

#ifndef VULKAN_ENGINE_SCENE_H
#define VULKAN_ENGINE_SCENE_H

#include <list>
#include <map>
#include <string>

class GameObject;
namespace SharkEngine::Core {
    class Scene {
    public:
        GameObject* GetRootGameObjects();

        int handle;
        bool isLoaded;
        std::map<std::string, GameObject*> sceneObject;
    };
}

#endif//VULKAN_ENGINE_SCENE_H
