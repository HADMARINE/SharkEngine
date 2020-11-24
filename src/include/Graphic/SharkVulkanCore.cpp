//
// Created by HADMARINE on 2020/11/24.
//

#include "SharkVulkan.hpp"

namespace SharkEngine::Core {

    SharkVulkan* SharkVulkan::getCore() {
        static SharkVulkan *core = new SharkVulkan();
        return core;
    }

    VkDevice *SharkVulkan::getDevice() {
        return nullptr;
    }

    std::vector<VkCommandBuffer> SharkVulkan::getCommandBuffers() {
        return this->commandBuffers;
    }


}// namespace SharkEngine::Core