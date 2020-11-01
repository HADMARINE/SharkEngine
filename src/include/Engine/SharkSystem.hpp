//
// Created by EunwooSong on 2020-10-23.
//

#ifndef VULKAN_ENGINE_SHARKSYSTEM_H
#define VULKAN_ENGINE_SHARKSYSTEM_H

#include "../../stdafx.hpp"
#include "SystemManager/SceneManager.h"
#include "SystemManager/TimeManager.h"

#define SHARK_ENGINE SharkSystem::Instance()

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
        timeMgr = new SharkEngine::Core::TimeManager();
        //inputMgr = new InputManager();
    };

    void Start()        { sceneMgr->Start(); };
    void Update()       { timeMgr->Update(); sceneMgr->Update(); };
    void LateUpdate()   { sceneMgr->LateUpdate(); };
    void Render()       { sceneMgr->Render(); };
    void EndScene()     { sceneMgr->EndScene(); };

    SharkEngine::Core::SceneManager* GetSceneManager() {return sceneMgr;};
    SharkEngine::Core::TimeManager* GetTimeManager() {return timeMgr;};
public:
    SharkEngine::Core::SceneManager* sceneMgr;
    SharkEngine::Core::TimeManager* timeMgr;
};

#endif//VULKAN_ENGINE_SHARKSYSTEM_H
