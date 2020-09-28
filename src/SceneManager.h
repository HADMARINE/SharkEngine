//
// Created by EunwooSong on 2020-09-28.
//

#ifndef VULKAN_ENGINE_SCENEMANAGER_H
#define VULKAN_ENGINE_SCENEMANAGER_H

#include <string>

class Scene;

class SceneManager {
public:
    Scene* currentScene;

    void ChangeScene(Scene* iter);
};


#endif//VULKAN_ENGINE_SCENEMANAGER_H
