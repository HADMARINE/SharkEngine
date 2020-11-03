//
// Created by EunwooSong on 2020-10-23.
//

#pragma once

#include "../../stdafx.hpp"
#include "SystemManager/SceneManager.h"
#include "SystemManager/TimeManager.h"

#define SHARK_ENGINE SharkSystem::Instance()
#define SHARK_SCENE_MGR SharkSystem::GetSceneManager()
#define SHARK_TIME_MGR SharkSystem::GetSceneManager()

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
        using namespace SharkEngine;

        sceneMgr = new SharkEngine::Core::SceneManager();
        timeMgr = new SharkEngine::Core::TimeManager();
        //inputMgr = new InputManager();
    };

    void Start()        { sceneMgr->Start(); };
    void Update()       { timeMgr->Update(); sceneMgr->Update(); };
    void LateUpdate()   { sceneMgr->LateUpdate(); };
    void Render()       { sceneMgr->Render(); };
    void EndScene()     { sceneMgr->EndScene(); };
    void Release()      { }

    SharkEngine::Core::SceneManager* GetSceneManager() {return sceneMgr;};
    SharkEngine::Core::TimeManager* GetTimeManager() {return timeMgr;};
public:
    SharkEngine::Core::SceneManager* sceneMgr;
    SharkEngine::Core::TimeManager* timeMgr;
};
