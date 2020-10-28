//
// Created by HADMARINE on 2020/09/20.
//

#include "include/Graphic/VulkanApplication.h"

std::vector<const char *> instanceExtensionNames = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
};

std::vector<const char *> layerNames = {
        "VK_LAYER_LUNARG_standard_validation"
};

std::vector<const char *> deviceExtensionNames = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

int main() {
    VulkanApplication* appObj = VulkanApplication::GetInstance();
    appObj->Initialize();
    appObj->Prepare();
    bool isWindowOpen = true;
    while (isWindowOpen) {
        appObj->Update();
        isWindowOpen = appObj->Render();
    }
    appObj->Release();
}