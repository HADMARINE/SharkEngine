//
// Created by HADMARINE on 2020/11/24.
//

#include "SharkVulkan.hpp"

namespace SharkEngine::Core {

    void SharkVulkan::FramebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto app = reinterpret_cast<SharkVulkan *>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }

    void *SharkVulkan::LoadTextures(std::string location) {
        return nullptr;
    }
}// namespace SharkEngine::Core