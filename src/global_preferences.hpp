//
// Created by HADMARINE on 2020/09/20.
//

#pragma once

#include <cstdlib>
#include <cstdint>

namespace GlobalPreferences {
    constexpr const char * APPLICATION_NAME = "SHARK_ENGINE_DEMO";
    constexpr const char * APPLICATION_VERSION = "1.0.0";
    constexpr const char * ENGINE_VERSION = "1.0.0";
    constexpr const char * ENGINE_NAME = "SharkEngine";

    constexpr const bool DEBUG = true;

    constexpr const uint32_t SCREEN_WIDTH = 800;
    constexpr const uint32_t SCREEN_HEIGHT = 600;

    constexpr const int MAX_FRAMES_IN_FLIGHT = 2;

    constexpr const bool enableValidationLayers = DEBUG;
}