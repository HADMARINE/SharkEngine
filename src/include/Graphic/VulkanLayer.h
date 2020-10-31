//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_VULKANLAYER_H
#define SHARKENGINE_VULKANLAYER_H

#pragma once
#include "IncludeVulkan.h"

struct LayerProperties{
    VkLayerProperties properties;
    std::vector<VkExtensionProperties> extensions;
};

//And Extension
class VulkanLayer {
public:
    VulkanLayer();
    ~VulkanLayer();

    std::vector<const char *>		appRequestedLayerNames;
    std::vector<const char *>		appRequestedExtensionNames;
    std::vector<LayerProperties>	layerPropertyList;

    VkResult GetInstanceLayerProperties();

    // Global extensions
    VkResult GetExtensionProperties(LayerProperties &layerProps, VkPhysicalDevice* gpu = nullptr);

    // Device based extensions
    VkResult GetDeviceExtensionProperties(VkPhysicalDevice* gpu);

    VkBool32 AreLayersSupported(std::vector<const char *> &);
    VkResult CreateDebugReportCallback();
    void DestroyDebugReportCallback();
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugFunction( VkFlags msgFlags,
                                                         VkDebugReportObjectTypeEXT objType,
                                                         uint64_t srcObject,
                                                         size_t location,
                                                         int32_t msgCode,
                                                         const char *layerPrefix,
                                                         const char *msg,
                                                         void *userData);
private:
    PFN_vkCreateDebugReportCallbackEXT dbgCreateDebugReportCallback;
    PFN_vkDestroyDebugReportCallbackEXT dbgDestroyDebugReportCallback;
    VkDebugReportCallbackEXT debugReportCallback;

public:
    VkDebugReportCallbackCreateInfoEXT dbgReportCreateInfo = {};
};


#endif//SHARKENGINE_VULKANLAYER_H
