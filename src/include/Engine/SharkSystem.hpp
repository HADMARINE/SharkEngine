//
// Created by EunwooSong on 2020-10-23.
//

#pragma once

#include "../../stdafx.hpp"
#include "SystemManager/SceneManager.h"
#include "SystemManager/TimeManager.h"

#include "../Graphic/DirectX9Core/DirectXCore.h"
#include "../Graphic/DirectX9Core/WindowsApplication.h"

#define SHARK_ENGINE    SharkSystem::Instance()
#define SHARK_SCENE_MGR SharkSystem::GetSceneManager()
#define SHARK_TIME_MGR  SharkSystem::GetSceneManager()
#define SHARK_DIRECT3D  DIRECTX_CORE
#define SHARK_WINDOWS   WINDOWS_APPLICATION

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

        SHARK_WINDOWS->Initialize();
        SHARK_WINDOWS->FloatWindow(SW_SHOWDEFAULT);
        SHARK_DIRECT3D->Initialize(SHARK_WINDOWS->GetHWND());
    };

    void Start()        { sceneMgr->Start(); }
    void Update()       { timeMgr->Update(); sceneMgr->Update(); }
    void LateUpdate()   { sceneMgr->LateUpdate(); }
    void Render()       { SHARK_DIRECT3D->BeginRender(); sceneMgr->Render(); SHARK_DIRECT3D->EndRender();}
    void EndScene()     { sceneMgr->EndScene(); }

    void Release()      {
        SAFE_DELETE(sceneMgr);
        SAFE_DELETE(timeMgr);


        SHARK_DIRECT3D->Release();
    }

    bool IsClosed() const { return SHARK_WINDOWS->CheckMessage().message == WM_QUIT; }

    SharkEngine::Core::SceneManager* GetSceneManager() {return sceneMgr;};
    SharkEngine::Core::TimeManager* GetTimeManager() {return timeMgr;};
public:
    SharkEngine::Core::SceneManager* sceneMgr;
    SharkEngine::Core::TimeManager* timeMgr;

private:

};
