//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_VULKANINSTANCE_H
#define SHARKENGINE_VULKANINSTANCE_H

#pragma once
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
    VkResult CreateInstance(std::vector<const char *>& layers);
    void DestroyInstance();

    // VulkanInstance Debug functions
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT
                                                        *pCallbackData,
                                                        void *pUserData);
};


#endif//SHARKENGINE_VULKANINSTANCE_H
