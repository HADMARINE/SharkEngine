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
#define SHARK_LINE_MGR      SHARK_ENGINE->SharkSystem::GetLineDebuggerManager()

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

        SHARK_WINDOWS->Initialize();
        SHARK_WINDOWS->FloatWindow(10);
        SHARK_DIRECT3D->Initialize(SHARK_WINDOWS->GetHWND());

        sceneMgr = new SharkEngine::Core::SceneManager();
        timeMgr = new SharkEngine::Core::TimeManager();
        textureMgr = new SharkEngine::Core::DirectXTextureManger();
        inputMgr = new SharkEngine::Core::InputManager();
        lineMgr = new SharkEngine::Core::LineDebuggerManager();

        //Initialize Managers
        timeMgr->Init();
    };

    void Start()        { sceneMgr->Start(); }
    bool Update()       {
        if(SHARK_WINDOWS->CheckMessage().message == WM_QUIT)
            return false;

        timeMgr->Update();
        inputMgr->Update();
        sceneMgr->Update();

        return true;
    }
    void LateUpdate()   { sceneMgr->LateUpdate(); }
    void Render()       {
        SHARK_DIRECT3D->BeginRender();

        SHARK_DIRECT3D->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
        sceneMgr->Render();
        SHARK_DIRECT3D->GetSprite()->End();

        SHARK_DIRECT3D->GetLine()->Begin();
        lineMgr->Render();
        SHARK_DIRECT3D->GetLine()->End();

        SHARK_DIRECT3D->EndRender();
    }
    void EndScene()     { sceneMgr->EndScene(); }

    void Release()      {
        textureMgr->Release();

        SAFE_DELETE(sceneMgr);
        SAFE_DELETE(timeMgr);
        SAFE_DELETE(textureMgr);
        SAFE_DELETE(inputMgr);
        SAFE_DELETE(lineMgr);

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
