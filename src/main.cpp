//
// Created by HADMARINE on 2020/09/20.
//

#include "include/Engine/Audio/SharkAudio.hpp"
#include "include/Graphic/VulkanApplication.h"

std::vector<const char *> layerNames = {
        "VK_LAYER_LUNARG_standard_validation"};

std::vector<const char *> deviceExtensionNames = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

int main() {
    SharkEngine::Audio::initialize();
    SharkEngine::Audio::SourcePreferences audioPref;
    SharkEngine::Audio audio("wavTester1.wav", audioPref);

    audio.play();
    VulkanApplication *appObj = VulkanApplication::GetInstance();
    appObj->Initialize();
    appObj->Prepare();
    bool isWindowOpen = true;
    while (isWindowOpen) {
        appObj->Update();
        isWindowOpen = appObj->Render();
    }
    appObj->Release();
}