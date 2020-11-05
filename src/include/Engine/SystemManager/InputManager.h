//
// Created by EunwooSong on 2020-10-23.
//

#ifndef VULKAN_ENGINE_INPUTMANAGER_HPP
#define VULKAN_ENGINE_INPUTMANAGER_HPP

#include <Windows.h>
#include "include/Engine/CoreDefine.h"

#define KEY_NONE    -1
#define KEY_DOWN    0
#define KEY_UP      1
#define KEY_STAY    2

namespace SharkEngine::Core {
    class InputManager {
    public:
        InputManager() {
            ZeroMemory(currentKeys, sizeof(currentKeys));
            ZeroMemory(lastKeys, sizeof(lastKeys));
        }
        ~InputManager() {}

        void Update();

        int GetKeyState(int vk) {
            if (currentKeys[vk] && lastKeys[vk]) return KEY_STAY;
            else if (currentKeys[vk] && !lastKeys[vk])
                return KEY_DOWN;
            else if (!currentKeys[vk] && lastKeys[vk])
                return KEY_UP;
            else
                return KEY_NONE;
        }

        Vec2 GetCursorPos() {
            return Vec2(cursorPos.x, cursorPos.y);
        }

    private:
        int currentKeys[256];
        int lastKeys[256];

        POINT cursorPos;
    };
}

#endif//VULKAN_ENGINE_INPUTMANAGER_HPP
