//
// Created by EunwooSong on 2020-10-23.
//

#ifndef VULKAN_ENGINE_SHARKSYSTEM_H
#define VULKAN_ENGINE_SHARKSYSTEM_H

#include "../../stdafx.hpp"
#include "SystemManager/SceneManager.h"

#define SHARK_ENGINE SharkSystem::Instance()
//#define SHARK_SCENE_MGR SharkSystem::sceneMgr
//#define SHARK_INPUT_MGR SharkSystem::sceneMgr

class SharkSystem {
public:
    static std::unique_ptr<SharkSystem> instance;
    static std::once_flag onlyOnce;

    SharkSystem() = default;
    ~SharkSystem() = default;

    static SharkSystem* Instance() {
        std::call_once(onlyOnce, [](){instance.reset(new SharkSystem()); });

        return instance.get();
    };

    void InitializeEngine() {
        sceneMgr = new SharkEngine::Core::SceneManager();
        //inputMgr = new InputManager();
    }

    template <typename T>
    void SetStartScene() {
        sceneMgr->ChangeScene<T>();
    }

    void Start()        { sceneMgr->Start(); }
    void Update()       { sceneMgr->Update(); }
    void LateUpdate()   { sceneMgr->LateUpdate(); }
    void Render()       { sceneMgr->Render(); }
    void EndScene()     { sceneMgr->EndScene(); }

public:
    SharkEngine::Core::SceneManager* sceneMgr;
    //static InputManager* inputMgr;
};

std::unique_ptr<SharkSystem> SharkSystem::instance;
std::once_flag SharkSystem::onlyOnce;

#endif//VULKAN_ENGINE_SHARKSYSTEM_H
