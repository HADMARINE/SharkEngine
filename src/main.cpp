//
// Created by HADMARINE on 2020/09/20.
//

#include "include/Graphic/VulkanApplication.h"
#include "include/Engine/SharkSystem.hpp"
#include "InGame/Scenes/TestScene.h"
#include "include/Engine/UniformBufferCamera/Camera.hpp"

using namespace SharkEngine::Core;

std::vector<const char *> layerNames = {
        "VK_LAYER_LUNARG_standard_validation"
};

std::vector<const char *> deviceExtensionNames = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

std::unique_ptr<SharkSystem> SharkSystem::instance;
std::once_flag SharkSystem::onlyOnce;

int main() {
    VulkanApplication* appObj = VulkanApplication::GetInstance();

    appObj->Initialize();
    appObj->Prepare();
    SHARK_ENGINE->InitializeEngine();
    SHARK_ENGINE->GetSceneManager()->ChangeScene(new TestScene());

    bool isWindowOpen = true;
    while (isWindowOpen) {
        SHARK_ENGINE->Start();
        appObj->Update();
        SHARK_ENGINE->Update();
        SHARK_ENGINE->LateUpdate();
        SHARK_ENGINE->Render();
        isWindowOpen = appObj->Render();

        SHARK_ENGINE->EndScene();
    }

    appObj->Release();
}