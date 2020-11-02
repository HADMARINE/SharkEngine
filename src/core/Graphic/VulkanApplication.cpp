//
// Created by EunwooSong on 2020-10-27.
//

#include "../../stdafx.hpp"
#include "../../Assets.hpp"
#include "../../include/Graphic/VulkanApplication.h"
#include "../../include/Graphic/VulkanDrawable.h"

std::unique_ptr<VulkanApplication> VulkanApplication::instance;
std::once_flag VulkanApplication::onlyOnce;

extern std::vector<const char *> layerNames;
extern std::vector<const char *> deviceExtensionNames;

VulkanApplication::VulkanApplication() {
    instanceObj.layerExtension.GetInstanceLayerProperties();

    deviceObj = NULL;
    debugFlag = false;
    rendererObj = NULL;
    isPrepared = false;
    isResizing = false;
}
VkResult VulkanApplication::CreateVulkanInstance(std::vector<const char *>& layers) {
    return instanceObj.CreateInstance(layers);
}
VkResult VulkanApplication::HandShakeWithDevice(VkPhysicalDevice *gpu, std::vector<const char *> &layers, std::vector<const char *> &extensions) {
    deviceObj = new VulkanDevice(gpu);

    if (!deviceObj){
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }

    deviceObj->layerExtension.GetDeviceExtensionProperties(gpu);
    vkGetPhysicalDeviceProperties(*gpu, &deviceObj->gpuProps);
    vkGetPhysicalDeviceMemoryProperties(*gpu, &deviceObj->memoryProperties);

    deviceObj->GetPhysicalDeviceQueuesAndProperties();

    deviceObj->GetGraphicsQueueHandle();

    // Create Logical Device
    return deviceObj->CreateDevice(layers, extensions);
}
VkResult VulkanApplication::EnumeratePhysicalDevices(std::vector<VkPhysicalDevice> &gpus) {
    uint32_t gpuDeviceCount;

    VkResult result = vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, NULL);

    gpuList.resize(gpuDeviceCount);

    result = vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, gpuList.data());

    return result;
}
VulkanApplication::~VulkanApplication() {
    delete rendererObj;
    rendererObj = NULL;
}
VulkanApplication *VulkanApplication::GetInstance() {
    std::call_once(onlyOnce, [](){instance.reset(new VulkanApplication()); });

    return instance.get();
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT
                                             *pCallbackData,
                                             void *pUserData) {
    CLogger::Debug("[VK] %s", pCallbackData->pMessage);
    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator,
                                      VkDebugUtilsMessengerEXT *pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VulkanApplication::Initialize() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    instanceObj.layerExtension.AreLayersSupported(layerNames);
    CreateVulkanInstance(layerNames);


    [&]() {
      if (!GlobalPreferences::enableValidationLayers) return;

      VkDebugUtilsMessengerCreateInfoEXT createInfo;

      createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      createInfo.messageSeverity =
              VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
              VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      createInfo.messageType =
              VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      createInfo.pfnUserCallback = debugCallback;

      if (CreateDebugUtilsMessengerEXT(instanceObj.instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
          CLogger::Error("Failed to set up debug message");
          throw std::runtime_error("Failed to set up debug messenger");
      }
    }();

    // Create the debugging report if debugging is enabled
    if (debugFlag) {
        instanceObj.layerExtension.CreateDebugReportCallback();
    }

    EnumeratePhysicalDevices(gpuList);

    if (gpuList.size() > 0) {
        HandShakeWithDevice(&gpuList[0], layerNames, deviceExtensionNames);
    }
    //Initialize GLFW
    if (!rendererObj) {
        rendererObj = new VulkanRenderer(this, deviceObj);
        rendererObj->CreatePresentationWindow();
        rendererObj->GetSwapChain()->InitializeSwapChain();
    }
    rendererObj->Initialize();
}
void VulkanApplication::Prepare() {
    isPrepared = false;
    rendererObj->Prepare();
    isPrepared = true;
}
void VulkanApplication::Update() {
    rendererObj->Update();
}
void VulkanApplication::Resize() {
    // If prepared then only proceed for
    if (!isPrepared) {
        return;
    }

    isResizing = true;

    vkDeviceWaitIdle(deviceObj->device);
    rendererObj->DestroyFrameBuffers();
    rendererObj->DestroyCommandPool();
    rendererObj->DestroyPipeline();
    rendererObj->GetPipelineObject()->DestroyPipelineCache();
    for (VulkanDrawable* drawableObj : *rendererObj->GetDrawingItems())
    {
        drawableObj->DestroyDescriptor();
    }
    rendererObj->DestroyRenderPass();
    rendererObj->GetSwapChain()->DestroySwapChain();
    rendererObj->DestroyDrawableVertexBuffer();
    rendererObj->DestroyDrawableUniformBuffer();
    rendererObj->DestroyTextureResource();
    rendererObj->DestroyDepthBuffer();
    rendererObj->Initialize();

    //Resize New Physical Window
    Prepare();

    isResizing = false;
}
bool VulkanApplication::Render() {
    if (!isPrepared)
        return false;

    return rendererObj->Render();
}
void VulkanApplication::Release() {
    // Destroy all the pipeline objects
    rendererObj->DestroyPipeline();

    rendererObj->GetPipelineObject()->DestroyPipelineCache();

    for (VulkanDrawable* drawableObj : *rendererObj->GetDrawingItems())
    {
        drawableObj->DestroyDescriptor();
    }

    rendererObj->GetShader()->DestroyShaders();
    rendererObj->DestroyFrameBuffers();
    rendererObj->DestroyRenderPass();
    rendererObj->DestroyDrawableVertexBuffer();
    rendererObj->DestroyDrawableUniformBuffer();

    rendererObj->DestroyDrawableCommandBuffer();
    rendererObj->DestroyDepthBuffer();
    rendererObj->GetSwapChain()->DestroySwapChain();
    rendererObj->DestroyCommandBuffer();
    rendererObj->DestroyDrawableSynchronizationObjects();
    rendererObj->DestroyCommandPool();
    rendererObj->DestroyPresentationWindow();
    rendererObj->DestroyTextureResource();
    deviceObj->DestroyDevice();
    if (debugFlag) {
        instanceObj.layerExtension.DestroyDebugReportCallback();
    }
    instanceObj.DestroyInstance();
}
