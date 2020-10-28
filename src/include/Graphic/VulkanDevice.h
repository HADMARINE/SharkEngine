//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_VULKANDEVICE_H
#define SHARKENGINE_VULKANDEVICE_H

#include "IncludeVulkan.h"
#include "VulkanLayer.h"

class VulkanApplication;

class VulkanDevice{
public:
    VulkanDevice(VkPhysicalDevice* gpu);
    ~VulkanDevice();

public:
    VkDevice							device;		// Logical device
    VkPhysicalDevice*					gpu;		// Physical device
    VkPhysicalDeviceProperties			gpuProps;	// Physical device attributes
    VkPhysicalDeviceMemoryProperties	memoryProperties;

public:
    // Queue
    VkQueue									queue;							// Vulkan Queues object
    std::vector<VkQueueFamilyProperties>	queueFamilyProps;				// Store all queue families exposed by the physical device. attributes
    uint32_t								graphicsQueueIndex;				// Stores graphics queue index
    uint32_t								graphicsQueueWithPresentIndex;  // Number of queue family exposed by device
    uint32_t								queueFamilyCount;				// Device specificc layer and extensions

    // Layer and extensions
    VulkanLayer		layerExtension;
    VkPhysicalDeviceFeatures	deviceFeatures;

public:
    VkResult CreateDevice(std::vector<const char *>& layers, std::vector<const char *>& extensions);
    void DestroyDevice();

    bool MemoryTypeFromProperties(uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex);

    // Get the avaialbe queues exposed by the physical devices
    void GetPhysicalDeviceQueuesAndProperties();

    uint32_t GetGraphicsQueueHandle();

    void GetDeviceQueue();

};

#endif//SHARKENGINE_VULKANDEVICE_H
