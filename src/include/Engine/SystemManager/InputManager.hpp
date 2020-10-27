//
// Created by EunwooSong on 2020-10-23.
//

#ifndef VULKAN_ENGINE_INPUTMANAGER_HPP
#define VULKAN_ENGINE_INPUTMANAGER_HPP

#include "../../../HadmarineEngine.hpp"


class InputManager {
public:
    int GetKeyState(int vk) {
        return glfwGetKey()
    }
};


#endif//VULKAN_ENGINE_INPUTMANAGER_HPP
