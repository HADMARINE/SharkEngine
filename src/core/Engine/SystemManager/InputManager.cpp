//
// Created by EunwooSong on 2020-11-05.
//
#include "include/Engine/SystemManager/InputManager.h"
#include "include/Engine/SharkSystem.hpp"

#include <WinUser.h>

using namespace SharkEngine::Core;

void InputManager::Update() {
    for (int i = 0; i < 256; i++) {
        lastKeys[i] = currentKeys[i];
        currentKeys[i] = ::GetKeyState(i) & 0x8000;
    }

    ::GetCursorPos(&cursorPos);
    ScreenToClient(SHARK_WINDOWS->GetHWND(), &cursorPos);
}