//
// Created by EunwooSong on 2020-10-13.
//

#ifndef VULKAN_ENGINE_SCENEMANAGER_H
#define VULKAN_ENGINE_SCENEMANAGER_H

namespace SharkEngine::Core {
    class Scene;

    class SceneManager {
    public:
        SceneManager() {};
        ~SceneManager() {};

        const Scene* GetCurrentScene();

        void Start();
        void Update();
        void LateUpdate();
        void Render();
        void EndScene();

        template <typename T>
        void ChangeScene();

    private:
        Scene* currentScene;
    };
}

#endif//VULKAN_ENGINE_SCENEMANAGER_H
