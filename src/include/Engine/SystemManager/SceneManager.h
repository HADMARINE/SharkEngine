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
        SceneManager() : currentScene(nullptr) {};
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

            if(this->currentScene) SAFE_DELETE(this->currentScene);
            this->currentScene = scene;
            currentScene->Init();
        }

    private:
        Scene* currentScene;
    };
}
