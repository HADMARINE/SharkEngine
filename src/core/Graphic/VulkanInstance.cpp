//
// Created by EunwooSong on 2020-10-27.
//


#include "../../include/Graphic/VulkanInstance.h"
#include "../../include/Graphic/VulkanApplication.h"
#include "../../Assets.hpp"
#include "../../GlobalPreferences.hpp"

bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : layerNames) {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            return false;
        }
    }
    return true;
};

std::vector<const char *> getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (GlobalPreferences::enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

VkResult  VulkanInstance::CreateInstance(std::vector<const char *>& layers) {
    //Extension
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

//    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    auto extensions = getRequiredExtensions();



    if (GlobalPreferences::enableValidationLayers) {
        if(!checkValidationLayerSupport()) {
            CLogger::Error("Validation layers requested, but not available");
            throw std::runtime_error("Validation layers requested, but not available");
        }
    }

    layerExtension.appRequestedExtensionNames	= extensions;
    layerExtension.appRequestedLayerNames		= layers;

    Assets::Parser::Semver appVer           = Assets::Parser::parseStringToSemver(GlobalPreferences::APPLICATION_VERSION);
    Assets::Parser::Semver engineVersion    = Assets::Parser::parseStringToSemver(GlobalPreferences::ENGINE_VERSION);

    VkApplicationInfo appInfo	= {};
    appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext				= NULL;
    appInfo.pApplicationName	= GlobalPreferences::APPLICATION_NAME;
    appInfo.applicationVersion	= VK_MAKE_VERSION(appVer.major, appVer.minor, appVer.patch);
    appInfo.pEngineName			= GlobalPreferences::ENGINE_NAME;
    appInfo.engineVersion		= VK_MAKE_VERSION(engineVersion.major, engineVersion.minor, engineVersion.patch);
    appInfo.apiVersion			= VK_API_VERSION_1_0;

    VkInstanceCreateInfo instInfo	= {};
    instInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pNext					= &layerExtension.dbgReportCreateInfo;
    instInfo.flags					= 0;
    instInfo.pApplicationInfo		= &appInfo;

//    //Layer
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (GlobalPreferences::enableValidationLayers) {
        instInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        instInfo.ppEnabledLayerNames = layers.data();

        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = debugCallback;

        instInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
    } else {
        instInfo.enabledLayerCount = 0;
        instInfo.pNext = nullptr;
    }

    instInfo.enabledExtensionCount	= static_cast<uint32_t>(extensions.size());
    instInfo.ppEnabledExtensionNames = extensions.data();

    VkResult result = vkCreateInstance(&instInfo, NULL, &instance);

    if(result != VK_SUCCESS) {
        CLogger::Error("Failed to create VKInstance");
        throw std::runtime_error("Failed to create VKInstance");
    }
    return result;
}

void VulkanInstance::DestroyInstance() {
    vkDestroyInstance(instance, NULL);
}
VkBool32 VulkanInstance::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
    CLogger::Debug("[VK] %s", pCallbackData->pMessage);
    return VK_FALSE;
}
