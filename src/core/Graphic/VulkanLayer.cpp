//
// Created by EunwooSong on 2020-10-27.
//

#include "../../include/Graphic/VulkanLayer.h"
#include "../../include/Graphic/VulkanApplication.h"
#include "../../CLogger.hpp"
VulkanLayer::VulkanLayer() {
    dbgCreateDebugReportCallback	= NULL;
    dbgDestroyDebugReportCallback	= NULL;
    debugReportCallback = NULL;
}
VulkanLayer::~VulkanLayer() {
    dbgCreateDebugReportCallback = NULL;
    dbgDestroyDebugReportCallback = NULL;
    debugReportCallback = NULL;
}
VkResult VulkanLayer::GetInstanceLayerProperties() {
    uint32_t						instanceLayerCount;		// Stores number of layers supported by instance
    std::vector<VkLayerProperties>	layerProperties;		// Vector to store layer properties
    VkResult						result;					// Variable to check Vulkan API result status

    // Query all the layers
    do {
        result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, NULL);

        if (result)
            return result;

        if (instanceLayerCount == 0)
            return VK_INCOMPLETE; // return fail

        layerProperties.resize(instanceLayerCount);
        result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
    } while (result == VK_INCOMPLETE);

    // Query all the extensions for each layer and store it.
    CLogger::Debug("\nInstanced Layers\n");
    CLogger::Debug("===================\n");
    for (auto globalLayerProp: layerProperties) {
        CLogger::Debug("\n%s \n\t|\n\t|---[Layer Name]--> %s ", globalLayerProp.description, globalLayerProp.layerName);

        LayerProperties layerProps;
        layerProps.properties = globalLayerProp;

        // Get Instance level extensions for corresponding layer properties
        result = GetExtensionProperties(layerProps);

        if (result){
            continue;
        }

        layerPropertyList.push_back(layerProps);

        for (auto j : layerProps.extensions){
            CLogger::Debug("\t\t|\n\t\t|---[Layer Extension]--> %s \n", j.extensionName);
        }
    }
    return result;
}
VkResult VulkanLayer::GetExtensionProperties(LayerProperties& layerProps, VkPhysicalDevice* gpu) {
    uint32_t	extensionCount;								 // Stores number of extension per layer
    VkResult	result;										 // Variable to check Vulkan API result status
    char*		layerName = layerProps.properties.layerName; // Name of the layer

    do {
        // Get the total number of extension in this layer
        if(gpu)
            result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, NULL);
        else
            result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, NULL);

        if (result || extensionCount == 0)
            continue;

        layerProps.extensions.resize(extensionCount);

        // Gather all extension properties
        if (gpu)
            result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, layerProps.extensions.data());
        else
            result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, layerProps.extensions.data());
    } while (result == VK_INCOMPLETE);

    return result;
}
VkResult VulkanLayer::GetDeviceExtensionProperties(VkPhysicalDevice* gpu) {
    VkResult result;					// Variable to check Vulkan API result status

    CLogger::Debug("\nDevice Extensions\n");
    CLogger::Debug("===================\n");
    VulkanApplication* appObj = VulkanApplication::GetInstance();
    std::vector<LayerProperties>* instanceLayerProp = &appObj->GetInstance()->instanceObj.layerExtension.layerPropertyList;

    for (auto globalLayerProp : *instanceLayerProp) {
        LayerProperties layerProps;
        layerProps.properties = globalLayerProp.properties;

        if (result = GetExtensionProperties(layerProps, gpu))
            continue;

        std::cout << "\n" << globalLayerProp.properties.description << "\n\t|\n\t|---[Layer Name]--> " << globalLayerProp.properties.layerName << "\n";
        layerPropertyList.push_back(layerProps);

        if (layerProps.extensions.size()) {
            for (auto j : layerProps.extensions) {
                CLogger::Debug("\t\t|\n\t\t|---[Device Extesion]--> %s \n", j.extensionName);
            }
        }
        else {
            CLogger::Debug("\t\t|\n\t\t|---[Device Extesion]--> No extension found \n");
        }
    }
    return result;
}
VkBool32 VulkanLayer::AreLayersSupported(std::vector<const char *> &layerNames) {
    uint32_t checkCount = (uint32_t)layerNames.size();
    uint32_t layerCount = (uint32_t)layerPropertyList.size();
    std::vector<const char*> unsupportedLayerNames;
    for (uint32_t i = 0; i < checkCount; i++) {
        VkBool32 isSupported = 0;
        for (uint32_t j = 0; j < layerCount; j++) {
            if (!strcmp(layerNames[i], layerPropertyList[j].properties.layerName)) {
                isSupported = 1;
            }
        }

        if (!isSupported) {
            CLogger::Debug("No Layer support found, removed from layer: %s", layerNames[i]);
            unsupportedLayerNames.push_back(layerNames[i]);
        }
        else {
            CLogger::Debug("Layer supported: %s \n", layerNames[i]);
        }
    }

    for (auto i : unsupportedLayerNames) {
        auto it = std::find(layerNames.begin(), layerNames.end(), i);
        if (it != layerNames.end()) layerNames.erase(it);
    }

    return true;
}
VkResult VulkanLayer::CreateDebugReportCallback() {
    VkResult result;

    VulkanApplication* appObj	= VulkanApplication::GetInstance();
    VkInstance* instance		= &appObj->instanceObj.instance;

    // Get vkCreateDebugReportCallbackEXT API
    dbgCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(*instance, "vkCreateDebugReportCallbackEXT");
    if (!dbgCreateDebugReportCallback) {
        CLogger::Error("GetInstanceProcAddr unable to locate vkCreateDebugReportCallbackEXT function. \n");
        return VK_ERROR_INITIALIZATION_FAILED;
    }
    CLogger::Debug("GetInstanceProcAddr loaded dbgCreateDebugReportCallback function\n");

    // Get vkDestroyDebugReportCallbackEXT API
    dbgDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(*instance, "vkDestroyDebugReportCallbackEXT");
    if (!dbgDestroyDebugReportCallback) {
        CLogger::Error("Error: GetInstanceProcAddr unable to locate vkDestroyDebugReportCallbackEXT function. \n");
        return VK_ERROR_INITIALIZATION_FAILED;
    }
    CLogger::Debug("GetInstanceProcAddr loaded dbgDestroyDebugReportCallback function\n");

    dbgReportCreateInfo.sType		= VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    dbgReportCreateInfo.pfnCallback = DebugFunction;
    dbgReportCreateInfo.pUserData	= NULL;
    dbgReportCreateInfo.pNext		= NULL;
    dbgReportCreateInfo.flags		= VK_DEBUG_REPORT_WARNING_BIT_EXT |
                                       VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
                                       VK_DEBUG_REPORT_ERROR_BIT_EXT |
                                       VK_DEBUG_REPORT_DEBUG_BIT_EXT;

    // Create the debug report callback and store the handle into 'debugReportCallback'
    result = dbgCreateDebugReportCallback(*instance, &dbgReportCreateInfo, NULL, &debugReportCallback);
    if (result == VK_SUCCESS) {
        CLogger::Debug("Debug report callback object created successfully\n");
    }

    return result;
}
void VulkanLayer::DestroyDebugReportCallback() {
    VulkanApplication* appObj   = VulkanApplication::GetInstance();
    VkInstance& instance	    = appObj->instanceObj.instance;
    dbgDestroyDebugReportCallback(instance, debugReportCallback, NULL);
}

VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanLayer::DebugFunction(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType, uint64_t srcObject, size_t location, int32_t msgCode, const char *layerPrefix, const char *msg, void *userData) {
        if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
            CLogger::Error("[VK_DEBUG_REPORT] ERROR: [%s] Code %d : %s \n", layerPrefix, (int)msgCode, msg);
        }
        else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
            CLogger::Debug("[VK_DEBUG_REPORT] WARNING: [%s] Code %d : %s \n", layerPrefix, (int)msgCode, msg);
        }
        else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
            CLogger::Info("[VK_DEBUG_REPORT] INFORMATION: [%s] Code %d : %s \n", layerPrefix, (int)msgCode, msg);
        }
        else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
            CLogger::Debug("[VK_DEBUG_REPORT] PERFORMANCE: [%s] Code %d : %s \n", layerPrefix, (int)msgCode, msg);
        }
        else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
            CLogger::Debug("[VK_DEBUG_REPORT] DEBUG: [%s] Code %d : %s \n", layerPrefix, (int)msgCode, msg);
        }
        else {
            return VK_FALSE;
        }

        fflush(stdout);
        return VK_TRUE;
}
