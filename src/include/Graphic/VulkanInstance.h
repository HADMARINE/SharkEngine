//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_VULKANINSTANCE_H
#define SHARKENGINE_VULKANINSTANCE_H

#include "VulkanLayer.h"

class VulkanInstance {
public:
    VulkanInstance(){}
    ~VulkanInstance(){}

public:
    // VulkanInstance member variables
    VkInstance instance;

    // Vulkan instance specific layer and extensions
    VulkanLayer layerExtension;

public:
    // VulkanInstance public functions
    VkResult CreateInstance(std::vector<const char *>& layers, std::vector<const char *>& extensions, const char* applicationName);
    void DestroyInstance();
};


#endif//SHARKENGINE_VULKANINSTANCE_H
