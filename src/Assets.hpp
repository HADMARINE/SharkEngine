//
// Created by HADMARINE on 2020/09/20.
//

#ifndef VULKAN_ENGINE_ASSETS_HPP
#define VULKAN_ENGINE_ASSETS_HPP

#include "stdafx.hpp"
#include <sstream>
#include <json/json.h>

namespace Assets {
    void pauseUntilKeyPressed(char, std::string);
    void pauseUntilKeyPressed(char k);
    void pauseUntilKeyPressed(std::string mes);
    void pauseUntilKeyPressed();

    int getRandomNumber(int, int);

    namespace Parser{
        std::wstring parseStringToWstring(const std::string);
        Json::Value parseStringToJson(const std::string);
        std::string parseJsonToString(const Json::Value);
    }
};

#endif //VULKAN_ENGINE_ASSETS_HPP
