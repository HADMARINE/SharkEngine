//
// Created by EunwooSong on 2020-10-23.
//

#ifndef VULKAN_ENGINE_SHARKSYSTEM_H
#define VULKAN_ENGINE_SHARKSYSTEM_H

#define SHARK_ENGINE SharkSystem::Initialize()
#define SHARK_SCENE_MGR SharkSystem::sceneMgr
#define SHARK_INPUT_MGR SharkSystem::sceneMgr

#include "SystemManager/InputManager.hpp"
#include "SystemManager/SceneManager.h"

using namespace SharkEngine::Core;

class SharkSystem {
public:
    SharkSystem() {

    }
    ~SharkSystem() {

    }

    static SharkSystem* Initialize() {
        static SharkSystem* sys = new SharkSystem();
        return sys;
    };

    void InitializeEngine() {
        sceneMgr = new SceneManager();
        inputMgr = new InputManager();
    }

    void MainLoop() {
        sceneMgr->MainLoop();
    }

public:
    static SceneManager* sceneMgr;
    static InputManager* inputMgr;
};

#endif//VULKAN_ENGINE_SHARKSYSTEM_H
