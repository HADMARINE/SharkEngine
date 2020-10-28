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
    GLFWwindow* window = appObj->rendererObj->window;

    result = glfwCreateWindowSurface(instance, window, nullptr, &scPublicVars.surface);

    if(result != VK_SUCCESS)
    {
        CLogger::Error("Failed to create window surface");
        throw std::runtime_error("Failed to create window surface");
    }

    return result;
}
uint32_t VulkanSwapChain::GetGraphicsQueueWithPresentationSupport() {
    VulkanDevice* device	= appObj->deviceObj;
    uint32_t queueCount		= device->queueFamilyCount;
    VkPhysicalDevice gpu	= *device->gpu;
    std::vector<VkQueueFamilyProperties>& queueProps = device->queueFamilyProps;

    VkBool32* supportsPresent = (VkBool32 *)malloc(queueCount * sizeof(VkBool32));
    for (uint32_t i = 0; i < queueCount; i++) {
        fpGetPhysicalDeviceSurfaceSupportKHR(gpu, i, scPublicVars.surface, &supportsPresent[i]);
    }

    uint32_t graphicsQueueNodeIndex = UINT32_MAX;
    uint32_t presentQueueNodeIndex = UINT32_MAX;
    for (uint32_t i = 0; i < queueCount; i++) {
        if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
            if (graphicsQueueNodeIndex == UINT32_MAX) {
                graphicsQueueNodeIndex = i;
            }

            if (supportsPresent[i] == VK_TRUE) {
                graphicsQueueNodeIndex = i;
                presentQueueNodeIndex = i;
                break;
            }
        }
    }

    if (presentQueueNodeIndex == UINT32_MAX) {
        for (uint32_t i = 0; i < queueCount; ++i) {
            if (supportsPresent[i] == VK_TRUE) {
                presentQueueNodeIndex = i;
                break;
            }
        }
    }

    free(supportsPresent);

    if (graphicsQueueNodeIndex == UINT32_MAX || presentQueueNodeIndex == UINT32_MAX) {
        return  UINT32_MAX;
    }

    return graphicsQueueNodeIndex;
}
void VulkanSwapChain::GetSurfaceCapabilitiesAndPresentMode() {
    VkResult  result;
    VkPhysicalDevice gpu = *appObj->deviceObj->gpu;
    result = fpGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, scPublicVars.surface, &scPrivateVars.surfCapabilities);
    assert(result == VK_SUCCESS);

    result = fpGetPhysicalDeviceSurfacePresentModesKHR(gpu, scPublicVars.surface, &scPrivateVars.presentModeCount, NULL);
    assert(result == VK_SUCCESS);

    scPrivateVars.presentModes.clear();
    scPrivateVars.presentModes.resize(scPrivateVars.presentModeCount);

    result = fpGetPhysicalDeviceSurfacePresentModesKHR(gpu, scPublicVars.surface, &scPrivateVars.presentModeCount, &scPrivateVars.presentModes[0]);
    assert(result == VK_SUCCESS);

    if (scPrivateVars.surfCapabilities.currentExtent.width == (uint32_t)-1)
    {
        // If the surface width and height is not defined, the set the equal to image size.
        scPrivateVars.swapChainExtent.width = rendererObj->width;
        scPrivateVars.swapChainExtent.height = rendererObj->height;
    }
    else
    {
        // If the surface size is defined, then it must match the swap chain size
        scPrivateVars.swapChainExtent = scPrivateVars.surfCapabilities.currentExtent;
    }
}
void VulkanSwapChain::ManagePresentMode() {
    scPrivateVars.swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (size_t i = 0; i < scPrivateVars.presentModeCount; i++) {
        if (scPrivateVars.presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            scPrivateVars.swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
        if ((scPrivateVars.swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) &&
            (scPrivateVars.presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
            scPrivateVars.swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
    }

    scPrivateVars.desiredNumberOfSwapChainImages = scPrivateVars.surfCapabilities.minImageCount + 1;
    if ((scPrivateVars.surfCapabilities.maxImageCount > 0) &&
        (scPrivateVars.desiredNumberOfSwapChainImages > scPrivateVars.surfCapabilities.maxImageCount))
    {
        // Application must settle for fewer images than desired:
        scPrivateVars.desiredNumberOfSwapChainImages = scPrivateVars.surfCapabilities.maxImageCount;
    }

    if (scPrivateVars.surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        scPrivateVars.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else {
        scPrivateVars.preTransform = scPrivateVars.surfCapabilities.currentTransform;
    }
}
void VulkanSwapChain::CreateSwapChainColorImages() {
    VkResult  result;
    VkSwapchainKHR oldSwapchain = scPublicVars.swapChain;

    VkSwapchainCreateInfoKHR swapChainInfo = {};
    swapChainInfo.sType					= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainInfo.pNext					= NULL;
    swapChainInfo.surface				= scPublicVars.surface;
    swapChainInfo.minImageCount			= scPrivateVars.desiredNumberOfSwapChainImages;
    swapChainInfo.imageFormat			= scPublicVars.format;
    swapChainInfo.imageExtent.width		= scPrivateVars.swapChainExtent.width;
    swapChainInfo.imageExtent.height	= scPrivateVars.swapChainExtent.height;
    swapChainInfo.preTransform			= scPrivateVars.preTransform;
    swapChainInfo.compositeAlpha		= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainInfo.imageArrayLayers		= 1;
    swapChainInfo.presentMode			= scPrivateVars.swapchainPresentMode;
    swapChainInfo.oldSwapchain			= oldSwapchain;
    swapChainInfo.clipped				= true;
    swapChainInfo.imageColorSpace		= VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    swapChainInfo.imageUsage			= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    swapChainInfo.imageSharingMode		= VK_SHARING_MODE_EXCLUSIVE;
    swapChainInfo.queueFamilyIndexCount = 0;
    swapChainInfo.pQueueFamilyIndices	= NULL;

    result = fpCreateSwapchainKHR(rendererObj->GetDevice()->device, &swapChainInfo, NULL, &scPublicVars.swapChain);
    assert(result == VK_SUCCESS);

    // Create the swapchain object
    result = fpGetSwapchainImagesKHR(rendererObj->GetDevice()->device, scPublicVars.swapChain, &scPublicVars.swapchainImageCount, NULL);
    assert(result == VK_SUCCESS);

    scPrivateVars.swapchainImages.clear();
    // Get the number of images the swapchain has
    scPrivateVars.swapchainImages.resize(scPublicVars.swapchainImageCount);
    assert(scPrivateVars.swapchainImages.size() >= 1);

    // Retrieve the swapchain image surfaces
    result = fpGetSwapchainImagesKHR(rendererObj->GetDevice()->device, scPublicVars.swapChain, &scPublicVars.swapchainImageCount, &scPrivateVars.swapchainImages[0]);
    assert(result == VK_SUCCESS);

    if (oldSwapchain != VK_NULL_HANDLE) {
        fpDestroySwapchainKHR(rendererObj->GetDevice()->device, oldSwapchain, NULL);
    }
}

void VulkanSwapChain::CreateColorImageView(VkCommandBuffer const &cmd) {
    VkResult  result;
    scPublicVars.colorBuffer.clear();
    for (uint32_t i = 0; i < scPublicVars.swapchainImageCount; i++) {
        SwapChainBuffer sc_buffer;

        VkImageViewCreateInfo imgViewInfo = {};
        imgViewInfo.sType			= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imgViewInfo.pNext			= NULL;
        imgViewInfo.format			= scPublicVars.format;
        imgViewInfo.components		= { VK_COMPONENT_SWIZZLE_IDENTITY };
        imgViewInfo.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
        imgViewInfo.subresourceRange.baseMipLevel	= 0;
        imgViewInfo.subresourceRange.levelCount		= 1;
        imgViewInfo.subresourceRange.baseArrayLayer	= 0;
        imgViewInfo.subresourceRange.layerCount		= 1;
        imgViewInfo.viewType						= VK_IMAGE_VIEW_TYPE_2D;
        imgViewInfo.flags							= 0;

        sc_buffer.image = scPrivateVars.swapchainImages[i];

        imgViewInfo.image = sc_buffer.image;

        result = vkCreateImageView(rendererObj->GetDevice()->device, &imgViewInfo, NULL, &sc_buffer.view);
        scPublicVars.colorBuffer.push_back(sc_buffer);
        assert(result == VK_SUCCESS);
    }
    scPublicVars.currentColorBuffer = 0;
}
