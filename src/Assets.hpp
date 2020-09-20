//
// Created by HADMARINE on 2020/09/20.
//

#ifndef VULKAN_ENGINE_ASSETS_HPP
#define VULKAN_ENGINE_ASSETS_HPP

#include "stdafx.hpp"

namespace Assets {
    void pauseUntilKeyPressed(char, std::string);
    void pauseUntilKeyPressed(char k);
    void pauseUntilKeyPressed(std::string mes);
    void pauseUntilKeyPressed();
    namespace Parser{
        static std::wstring parseStringToWstring(const std::string);
    }
};

#endif //VULKAN_ENGINE_ASSETS_HPP
