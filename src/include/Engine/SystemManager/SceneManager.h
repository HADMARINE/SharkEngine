//
// Created by EunwooSong on 2020-10-13.
//

#pragma once

#include "../../../include/Engine/CoreDefine.h"
#include "../../../CLogger.hpp"
#include "../Scene/Scene.h"

namespace SharkEngine::Core {
    class SceneManager {
    public:
        SceneManager() : currentScene(nullptr), nextScene(nullptr) {};
        ~SceneManager() {};

        Scene* GetCurrentScene();

        void Start();
        void Update();
        void LateUpdate();
        void Render();
        void EndScene();

        void ChangeScene(Scene* scene) {
            if(!scene) {
                throw std::runtime_error("This is not supported type(Scene Manager)");
            }

            if(currentScene) {
                nextScene = scene;
            }
            else {
                currentScene = scene;
                currentScene->Init();
            }
        }

    private:
        Scene* currentScene;
        Scene* nextScene;
    };
}
