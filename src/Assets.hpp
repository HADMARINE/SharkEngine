//
// Created by HADMARINE on 2020/09/20.
//

#pragma once

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
      [[maybe_unused]] std::wstring parseStringToWstring(const std::string&);
      [[maybe_unused]] Json::Value parseStringToJson(const std::string&);
        std::string parseJsonToString(const Json::Value&);
    }
};
