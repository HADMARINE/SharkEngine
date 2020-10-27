//
// Created by HADMARINE on 2020/09/20.
//

#pragma once

#include "stdafx.hpp"
#include <json/json.h>
#include <sstream>

namespace Assets {
    void pauseUntilKeyPressed(char, std::string);
    void pauseUntilKeyPressed(char k);
    void pauseUntilKeyPressed(std::string mes);
    void pauseUntilKeyPressed();

    int getRandomNumber(int, int);

    namespace Parser {
        struct Semver {
            int major;
            int minor;
            int patch;
        };

        std::wstring parseStringToWstring(const std::string &);
        Json::Value parseStringToJson(const std::string &);
        std::string parseJsonToString(const Json::Value &);
        Semver parseStringToSemver(std::string);

        std::list<std::string> splitStringByDelimiter(std::string, std::string);

    }// namespace Parser
};   // namespace Assets