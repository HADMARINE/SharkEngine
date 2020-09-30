//
// Created by EunwooSong on 2020-09-28.
//

#ifndef VULKAN_ENGINE_SCENEMANAGER_H
#define VULKAN_ENGINE_SCENEMANAGER_H

#include <string>
#define SceneMGR SharkEngine::Core::SceneManager::Instance()

namespace SharkEngine::Core {
    class Scene;

    class SceneManager {
    public:
        Scene *currentScene;
        Scene *beforeScene;

        static SceneManager *Instance();

        void Update();
        void LateUpdate();
        void Render();
        void EndScene();

        void ChangeScene(Scene *iter);
    };
}

#endif//VULKAN_ENGINE_SCENEMANAGER_H
