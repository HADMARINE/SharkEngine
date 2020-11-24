//
// Created by HADMARINE on 2020/11/24.
//

#include "SharkVulkan.hpp"

namespace SharkEngine::Core {

    SharkVulkan *SharkVulkan::GetCore() {
        static SharkVulkan *core = new SharkVulkan();
        return core;
    }

    VkDevice *SharkVulkan::GetDevice() {
        return &this->device;
    }

    std::vector<VkCommandBuffer> SharkVulkan::GetCommandBuffers() {
        return this->commandBuffers;
    }

    std::vector<VkImage> SharkVulkan::GetSwapChainImages() {
        return this->swapChainImages;
    }

    VkDescriptorPool SharkVulkan::GetDescriptorPool() {
        return this->descriptorPool;
    }

    VkDescriptorSetLayout SharkVulkan::GetDescriptorSetLayout() {
        return this->descriptorSetLayout;
    }

    std::vector<VkBuffer> SharkVulkan::GetUniformBuffers() {
        return this->uniformBuffers;
    }

    VkSampler SharkVulkan::GetTextureSampler() {
        return this->textureSampler;
    }


}// namespace SharkEngine::Core