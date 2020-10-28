//
// Created by EunwooSong on 2020-10-27.
//

#include "../../include/Graphic/VulkanInstance.h"
VkResult  VulkanInstance::CreateInstance(std::vector<const char *> &layers, std::vector<const char *> &extensionNames, const char *applicationName) {
    layerExtension.appRequestedExtensionNames	= extensionNames;
    layerExtension.appRequestedLayerNames		= layers;

    VkApplicationInfo appInfo	= {};
    appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext				= NULL;
    appInfo.pApplicationName	= applicationName;
    appInfo.applicationVersion	= 1;
    appInfo.pEngineName			= applicationName;
    appInfo.engineVersion		= 1;
    appInfo.apiVersion			= VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo instInfo	= {};
    instInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pNext					= &layerExtension.dbgReportCreateInfo;
    instInfo.flags					= 0;
    instInfo.pApplicationInfo		= &appInfo;

    instInfo.enabledLayerCount		= (uint32_t)layers.size();
    instInfo.ppEnabledLayerNames	= layers.size() ? layers.data() : NULL;

    instInfo.enabledExtensionCount	= (uint32_t)extensionNames.size();
    instInfo.ppEnabledExtensionNames = extensionNames.size() ? extensionNames.data() : NULL;

    VkResult result = vkCreateInstance(&instInfo, NULL, &instance);
    assert(result == VK_SUCCESS);

    return result;
}
void VulkanInstance::DestroyInstance() {
    vkDestroyInstance(instance, NULL);
}
