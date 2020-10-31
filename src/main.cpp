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
    float xPos = 1.0f;
    audioPref.pos = glm::vec3(xPos, 0.0f, 0.0f);
    audioPref.loop = true;

    SharkEngine::Audio audio("helloworld.wav", audioPref);
    audio.play();


    for (int i = 0; i < 20; i++) {
        sleep(1);
        xPos -= 0.1f;
        audioPref.pos = glm::vec3(xPos, 0.0f, 0.0f);
        audio.updateSourcePreferences(audioPref);
    }

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