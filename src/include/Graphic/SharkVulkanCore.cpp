//
// Created by HADMARINE on 2020/11/24.
//

#include "SharkVulkan.hpp"

namespace SharkEngine::Core {

    void SharkVulkan::Run() {

    }

    void SharkVulkan::InitWindow() {
        glfwInit();

        // GLFW SETTINGS
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(
                GlobalPreferences::SCREEN_WIDTH,
                GlobalPreferences::SCREEN_HEIGHT,
                GlobalPreferences::APPLICATION_NAME,
                nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, SharkVulkan::FramebufferResizeCallback);
    }

}// namespace SharkEngine::Core