//
// Created by EunwooSong on 2020-10-23.
//

#pragma once

#include "../../stdafx.hpp"
#include "../Graphic/DirectX9Core/DirectXCore.h"
#include "../Graphic/DirectX9Core/WindowsApplication.h"
#include "SystemManager/DirectXTextureManger.h"
#include "SystemManager/InputManager.h"
#include "SystemManager/LineDebuggerManager.h"
#include "SystemManager/SceneManager.h"
#include "SystemManager/TimeManager.h"

#define SHARK_ENGINE        SharkSystem::Instance()
#define SHARK_DIRECT3D      DIRECTX_CORE
#define SHARK_WINDOWS       WINDOWS_APPLICATION

#define SHARK_TIME_MGR      SHARK_ENGINE->SharkSystem::GetTimeManager()
#define SHARK_SCENE_MGR     SHARK_ENGINE->SharkSystem::GetSceneManager()
#define SHARK_TEXTURE_MGR   SHARK_ENGINE->SharkSystem::GetTextureManager()
#define SHARK_LINE_MGR   SHARK_ENGINE->SharkSystem::GetLineDebuggerManager()

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
        textureMgr = new SharkEngine::Core::DirectXTextureManger();
        inputMgr = new SharkEngine::Core::InputManager();

        SHARK_WINDOWS->Initialize();
        SHARK_WINDOWS->FloatWindow(10);
        SHARK_DIRECT3D->Initialize(SHARK_WINDOWS->GetHWND());
    };

    void Start()        { sceneMgr->Start(); }
    void Update()       { timeMgr->Update(); inputMgr->Update(); sceneMgr->Update(); }
    void LateUpdate()   { sceneMgr->LateUpdate(); }
    void Render()       { SHARK_DIRECT3D->BeginRender(); sceneMgr->Render(); SHARK_DIRECT3D->EndRender(); lineMgr->Render();}
    void EndScene()     { sceneMgr->EndScene(); }

    void Release()      {
        textureMgr->Release();

        SAFE_DELETE(sceneMgr);
        SAFE_DELETE(timeMgr);
        SAFE_DELETE(textureMgr);

        SHARK_DIRECT3D->Release();
    }

    bool IsClosed() const { return SHARK_WINDOWS->CheckMessage().message == WM_QUIT; }

    SharkEngine::Core::SceneManager* GetSceneManager() {return sceneMgr;};
    SharkEngine::Core::TimeManager* GetTimeManager() {return timeMgr;};
    SharkEngine::Core::DirectXTextureManger* GetTextureManager() {return textureMgr;};
    SharkEngine::Core::LineDebuggerManager* GetLineDebuggerManager() {return lineMgr;}
public:
    SharkEngine::Core::SceneManager* sceneMgr;
    SharkEngine::Core::TimeManager* timeMgr;
    SharkEngine::Core::InputManager* inputMgr;
    SharkEngine::Core::DirectXTextureManger* textureMgr;
    SharkEngine::Core::LineDebuggerManager* lineMgr;

};
