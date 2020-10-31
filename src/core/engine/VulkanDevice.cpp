//
// Created by EunwooSong on 2020-10-27.
//

#include "../../include/Graphic/VulkanDevice.h"
VulkanDevice::VulkanDevice(VkPhysicalDevice *physicalDevice) {
    gpu = physicalDevice;
}
VulkanDevice::~VulkanDevice() {
}
VkResult VulkanDevice::CreateDevice(std::vector<const char *> &layers, std::vector<const char *> &extensions) {
    layerExtension.appRequestedLayerNames		= layers;
    layerExtension.appRequestedExtensionNames	= extensions;

    // Create Device with available queue information.

    VkResult result;
    float queuePriorities[1]			= { 0.0 };
    VkDeviceQueueCreateInfo queueInfo	= {};
    queueInfo.queueFamilyIndex			= graphicsQueueIndex;
    queueInfo.sType						= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext						= NULL;
    queueInfo.queueCount				= 1;
    queueInfo.pQueuePriorities			= queuePriorities;


    vkGetPhysicalDeviceFeatures(*gpu, &deviceFeatures);

    VkPhysicalDeviceFeatures setEnabledFeatures = {VK_FALSE};
    setEnabledFeatures.samplerAnisotropy = deviceFeatures.samplerAnisotropy;

    VkDeviceCreateInfo deviceInfo		= {};
    deviceInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext					= NULL;
    deviceInfo.queueCreateInfoCount		= 1;
    deviceInfo.pQueueCreateInfos		= &queueInfo;
    deviceInfo.enabledLayerCount		= 0;
    deviceInfo.ppEnabledLayerNames		= NULL;											// Device layers are deprecated
    deviceInfo.enabledExtensionCount	= (uint32_t)extensions.size();
    deviceInfo.ppEnabledExtensionNames	= extensions.size() ? extensions.data() : NULL;
    deviceInfo.pEnabledFeatures			= &setEnabledFeatures;

    result = vkCreateDevice(*gpu, &deviceInfo, NULL, &device);
    assert(result == VK_SUCCESS);

    return result;
}
void VulkanDevice::DestroyDevice() {
}
bool VulkanDevice::MemoryTypeFromProperties(uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex) {
    for (uint32_t i = 0; i < 32; i++) {
        if ((typeBits & 1) == 1) {
            if ((memoryProperties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }

    return false;

}
void VulkanDevice::GetPhysicalDeviceQueuesAndProperties() {
    vkGetPhysicalDeviceQueueFamilyProperties(*gpu, &queueFamilyCount, NULL);

    queueFamilyProps.resize(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(*gpu, &queueFamilyCount, queueFamilyProps.data());
}
uint32_t VulkanDevice::GetGraphicsQueueHandle() {
    bool found = false;

    for (unsigned int i = 0; i < queueFamilyCount; i++) {

        if (queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            found = true;
            graphicsQueueIndex = i;
            break;
        }
    }
    return 0;
}
void VulkanDevice::GetDeviceQueue() {
    vkGetDeviceQueue(device, graphicsQueueWithPresentIndex, 0, &queue);
}
