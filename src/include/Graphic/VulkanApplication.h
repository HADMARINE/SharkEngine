//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_VULKANAPPLICATION_H
#define SHARKENGINE_VULKANAPPLICATION_H

#pragma once
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanRenderer.h"
#include "VulkanLayer.h"

class VulkanApplication {
private:
    static std::unique_ptr<VulkanApplication> instance;
    static std::once_flag onlyOnce;

    VulkanApplication();

    // Create the Vulkan instance object
    VkResult CreateVulkanInstance(std::vector<const char *>& layers);
    VkResult HandShakeWithDevice(VkPhysicalDevice* gpu, std::vector<const char *>& layers, std::vector<const char *>& extensions);
    VkResult EnumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpus);

public:
    ~VulkanApplication();

    static VulkanApplication* GetInstance();

    // Life cycle
    void Initialize();				// Initialize and allocate resources
    void Prepare();					// Prepare resource
    void Update();					// Update data
    void Resize();					// Resize window
    bool Render();					// Render primitives
    void Release();			        // Release resources

    VulkanInstance  instanceObj;	// Vulkan Instance object
    VulkanDevice*   deviceObj;
    VulkanRenderer* rendererObj;
    VkDebugUtilsMessengerEXT debugMessenger;
    bool isPrepared;
    bool isResizing;

private:
    bool debugFlag;
    std::vector<VkPhysicalDevice> gpuList;
};


#endif//SHARKENGINE_VULKANAPPLICATION_H
