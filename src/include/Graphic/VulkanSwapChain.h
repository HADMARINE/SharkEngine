//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_VULKANSWAPCHAIN_H
#define SHARKENGINE_VULKANSWAPCHAIN_H

#pragma once
#include "IncludeVulkan.h"
#include "VulkanStructs.h"

class VulkanInstance;
class VulkanDevice;
class VulkanRenderer;
class VulkanApplication;

/*************** SWAP CHAIN STRUCTURE **************/
struct SwapChainBuffer{
    VkImage image;
    VkImageView view;
};

struct SwapChainPrivateVariables
{
    // Store the image surface capabilities
    VkSurfaceCapabilitiesKHR	surfCapabilities;

    // Stores the number of present mode support by the implementation
    uint32_t					presentModeCount;

    // Arrays for retrived present modes
    std::vector<VkPresentModeKHR> presentModes;

    // Size of the swap chain color images
    VkExtent2D					swapChainExtent;

    // Number of color images supported by the implementation
    uint32_t					desiredNumberOfSwapChainImages;

    VkSurfaceTransformFlagBitsKHR preTransform;

    // Stores present mode bitwise flag for the creation of swap chain
    VkPresentModeKHR			swapchainPresentMode;

    // The retrived drawing color swap chain images
    std::vector<VkImage>		swapchainImages;

    std::vector<VkSurfaceFormatKHR> surfFormats;
};

struct SwapChainPublicVariables
{
    // The logical platform dependent surface object
    VkSurfaceKHR surface;

    // Number of buffer image used for swap chain
    uint32_t swapchainImageCount;

    // Swap chain object
    VkSwapchainKHR swapChain;

    // List of color swap chain images
    std::vector<SwapChainBuffer> colorBuffer;

    // Semaphore for sync purpose
    VkSemaphore presentCompleteSemaphore;

    // Current drawing surface index in use
    uint32_t currentColorBuffer;

    // Format of the image
    VkFormat format;
};

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
