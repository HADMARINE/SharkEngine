//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_VULKANSWAPCHAIN_H
#define SHARKENGINE_VULKANSWAPCHAIN_H

#include "IncludeVulkan.h"
#include "VulkanStructs.h"

class VulkanInstance;
class VulkanDevice;
class VulkanRenderer;
class VulkanApplication;

class VulkanSwapChain {
public:
    VulkanSwapChain(VulkanRenderer* renderer);
    ~VulkanSwapChain();
    void InitializeSwapChain();
    void CreateSwapChain(const VkCommandBuffer& cmd);
    void DestroySwapChain();
    void SetSwapChainExtent(uint32_t swapChainWidth, uint32_t swapChainHeight);

private:
    VkResult CreateSwapChainExtensions();
    void GetSupportedFormats();
    VkResult CreateSurface();
    uint32_t GetGraphicsQueueWithPresentationSupport();
    void GetSurfaceCapabilitiesAndPresentMode();
    void ManagePresentMode();
    void CreateSwapChainColorImages();
    void CreateColorImageView(const VkCommandBuffer& cmd);


public:
    SwapChainPublicVariables	scPublicVars;
    PFN_vkQueuePresentKHR		fpQueuePresentKHR;
    PFN_vkAcquireNextImageKHR	fpAcquireNextImageKHR;

private:
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR		fpGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		fpGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	fpGetPhysicalDeviceSurfacePresentModesKHR;
    PFN_vkDestroySurfaceKHR							fpDestroySurfaceKHR;

    // Layer Extensions Debugging
    PFN_vkCreateSwapchainKHR	fpCreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR	fpDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;

    SwapChainPrivateVariables	scPrivateVars;
    VulkanRenderer*				rendererObj;	// parent
    VulkanApplication*			appObj;
};


#endif//SHARKENGINE_VULKANSWAPCHAIN_H
