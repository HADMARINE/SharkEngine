//
// Created by EunwooSong on 2020-10-27.
//

#include "../../include/Graphic/IncludeVulkan.h"

#include "../../include/Graphic/VulkanSwapChain.h"
#include "../../include/Graphic/VulkanInstance.h"
#include "../../include/Graphic/VulkanRenderer.h"
#include "../../include/Graphic/VulkanApplication.h"
#include "../../include/Graphic/VulkanStructs.h"
#include "../../CLogger.hpp"
#include "../../stdafx.hpp"

#define INSTANCE_FUNC_PTR(instance, entrypoint){											\
    fp##entrypoint = (PFN_vk##entrypoint) vkGetInstanceProcAddr(instance, "vk"#entrypoint); \
    if (fp##entrypoint == NULL) {															\
        CLogger::Error("Unable to locate the vkGetDeviceProcAddr: vk"#entrypoint);			\
        exit(-1);																			\
    }																						\
}

#define DEVICE_FUNC_PTR(dev, entrypoint){													\
    fp##entrypoint = (PFN_vk##entrypoint) vkGetDeviceProcAddr(dev, "vk"#entrypoint);		\
    if (fp##entrypoint == NULL) {															\
        CLogger::Error("Unable to locate the vkGetDeviceProcAddr: vk"#entrypoint);			 \
        exit(-1);																			\
    }																						\
}

VulkanSwapChain::VulkanSwapChain(VulkanRenderer *renderer) {
    rendererObj = renderer;
    appObj		= VulkanApplication::GetInstance();
    scPublicVars.swapChain = VK_NULL_HANDLE;
}
VulkanSwapChain::~VulkanSwapChain() {
    scPrivateVars.swapchainImages.clear();
    scPrivateVars.surfFormats.clear();
    scPrivateVars.presentModes.clear();
}
void VulkanSwapChain::InitializeSwapChain() {
    CreateSwapChainExtensions();

    // Create surface and associate with Windowing
    CreateSurface();

    // Getting a graphics queue with presentation support
    uint32_t index = GetGraphicsQueueWithPresentationSupport();
    if (index == UINT32_MAX)
    {
        std::cout << "Could not find a graphics and a present queue\nCould not find a graphics and a present queue\n";
        exit(-1);
    }
    rendererObj->GetDevice()->graphicsQueueWithPresentIndex = index;

    // Get the list of formats that are supported
    GetSupportedFormats();
}
void VulkanSwapChain::CreateSwapChain(VkCommandBuffer const &cmd) {
    GetSurfaceCapabilitiesAndPresentMode();
    ManagePresentMode();
    CreateSwapChainColorImages();
    CreateColorImageView(cmd);
}
void VulkanSwapChain::DestroySwapChain() {
    VulkanDevice* deviceObj = appObj->deviceObj;

    for (uint32_t i = 0; i < scPublicVars.swapchainImageCount; i++) {
        vkDestroyImageView(deviceObj->device, scPublicVars.colorBuffer[i].view, NULL);
    }

    if (!appObj->isResizing) {
        fpDestroySwapchainKHR(deviceObj->device, scPublicVars.swapChain, NULL);
        vkDestroySurfaceKHR(appObj->instanceObj.instance, scPublicVars.surface, NULL);
    }
}
void VulkanSwapChain::SetSwapChainExtent(uint32_t swapChainWidth, uint32_t swapChainHeight) {
    scPrivateVars.swapChainExtent.width = swapChainWidth;
    scPrivateVars.swapChainExtent.height = swapChainHeight;
}
VkResult VulkanSwapChain::CreateSwapChainExtensions() {
    // Dependency on createPresentationWindow()
    VkInstance& instance	= appObj->instanceObj.instance;
    VkDevice& device		= appObj->deviceObj->device;

    // Get Instance based swap chain extension function pointer
    INSTANCE_FUNC_PTR(instance, GetPhysicalDeviceSurfaceSupportKHR);
    INSTANCE_FUNC_PTR(instance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
    INSTANCE_FUNC_PTR(instance, GetPhysicalDeviceSurfaceFormatsKHR);
    INSTANCE_FUNC_PTR(instance, GetPhysicalDeviceSurfacePresentModesKHR);
    INSTANCE_FUNC_PTR(instance, DestroySurfaceKHR);

    // Get Device based swap chain extension function pointer
    DEVICE_FUNC_PTR(device, CreateSwapchainKHR);
    DEVICE_FUNC_PTR(device, DestroySwapchainKHR);
    DEVICE_FUNC_PTR(device, GetSwapchainImagesKHR);
    DEVICE_FUNC_PTR(device, AcquireNextImageKHR);
    DEVICE_FUNC_PTR(device, QueuePresentKHR);

    return VK_SUCCESS;
}
void VulkanSwapChain::GetSupportedFormats() {
    VkPhysicalDevice gpu = *rendererObj->GetDevice()->gpu;

    // Get the list of VkFormats that are supported:
    uint32_t formatCount;
    fpGetPhysicalDeviceSurfaceFormatsKHR(gpu, scPublicVars.surface, &formatCount, NULL);

    scPrivateVars.surfFormats.clear();
    scPrivateVars.surfFormats.resize(formatCount);

    // Get VkFormats in allocated objects
    fpGetPhysicalDeviceSurfaceFormatsKHR(gpu, scPublicVars.surface, &formatCount, &scPrivateVars.surfFormats[0]);

    if (formatCount == 1 && scPrivateVars.surfFormats[0].format == VK_FORMAT_UNDEFINED) {
        scPublicVars.format = VK_FORMAT_B8G8R8A8_UNORM;
    } else {
        scPublicVars.format = scPrivateVars.surfFormats[0].format;
    }
}
VkResult VulkanSwapChain::CreateSurface() {
    VkResult  result;

    VkInstance& instance = appObj->instanceObj.instance;

#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType		= VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext		= NULL;
    createInfo.hinstance	= rendererObj->connection;
    createInfo.hwnd			= rendererObj->window;

    result = vkCreateWin32SurfaceKHR(instance, &createInfo, NULL, &scPublicVars.surface);

#else  // _WIN32

    VkXcbSurfaceCreateInfoKHR createInfo = {};
	createInfo.sType		= VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext		= NULL;
	createInfo.connection	= rendererObj->connection;
	createInfo.window		= rendererObj->window;

	result = vkCreateXcbSurfaceKHR(instance, &createInfo, NULL, &surface);
#endif // _WIN32

    assert(result == VK_SUCCESS);
    return result;
}
uint32_t VulkanSwapChain::GetGraphicsQueueWithPresentationSupport() {
    return 0;
}
void VulkanSwapChain::GetSurfaceCapabilitiesAndPresentMode() {
}
void VulkanSwapChain::ManagePresentMode() {
}
void VulkanSwapChain::CreateSwapChainColorImages() {
}
void VulkanSwapChain::CreateColorImageView(VkCommandBuffer const &cmd) {
}
