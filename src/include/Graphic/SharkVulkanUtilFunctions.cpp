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

    uint32_t SharkVulkan::AppendDrawable(const SharkVulkanDrawable& drawable) {
        this->drawables.push_back(drawable);
        return this->drawables.size() - 1;
    }

    void SharkVulkan::DeleteDrawable(uint32_t pos) {
        this->drawables.erase(pos);
    }

}// namespace SharkEngine::Core