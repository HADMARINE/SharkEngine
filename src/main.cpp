//
// Created by HADMARINE on 2020/09/20.
//

//#include "include/Graphic/VulkanApplication.h"
#include "include/Graphic/VulkanGraphicCore.h"
#include "InGame/Scenes/TestScene.h"
#include "include/Engine/SharkSystem.hpp"
#include "include/Graphic/VulkanDrawableManager.h"
#include "include/Graphic/VulkanDrawable.h"
#include "include/Graphic/VulkanTextureManager.h"
//#include "include/Engine/UniformBufferCamera/Camera.hpp"

using namespace SharkEngine::Core;
using namespace VulkanCore;
//std::vector<const char *> layerNames = {
////        "VK_LAYER_LUNARG_standard_validation"
//        "VK_LAYER_KHRONOS_validation"
//};
//
//std::vector<const char *> deviceExtensionNames = {
//        VK_KHR_SWAPCHAIN_EXTENSION_NAME
//};

std::unique_ptr<SharkSystem> SharkSystem::instance;
std::once_flag SharkSystem::onlyOnce;

int main() {
//    VulkanApplication* appObj = VulkanApplication::GetInstance();
//
//    appObj->Initialize();
//    appObj->Prepare();

    //ROLLBACK_GAME_PROJECT
    VULKAN_CORE->Initialize();
    SHARK_ENGINE->InitializeEngine();

    VulkanDrawable* tmp =  new VulkanDrawable(VULKAN_CORE,
                                              VULKAN_TEXTURE_MANAGER->GetTextureImage("texture.jpg"),
                                             VULKAN_CORE->vertices, std::vector<uint16_t>{0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4});

    VulkanDrawable* tmp_2 =  new VulkanDrawable(VULKAN_CORE,
                                              VULKAN_TEXTURE_MANAGER->GetTextureImage("viking_room.png"),
                                              VULKAN_CORE->vertices_2, std::vector<uint16_t>{0, 1, 2, 2, 3, 0});


    VULKAN_DRAW_MANAGER->AddVulkanDrawable(tmp);
    VULKAN_DRAW_MANAGER->AddVulkanDrawable(tmp_2);

    SHARK_ENGINE->GetSceneManager()->ChangeScene(new TestScene());

    bool isWindowOpen = true;
    while (isWindowOpen) {
        VULKAN_DRAW_MANAGER->CheckInit();

        SHARK_ENGINE->Start();
        //appObj->Update();
        SHARK_ENGINE->Update();
        SHARK_ENGINE->LateUpdate();
        SHARK_ENGINE->Render();

        VULKAN_DRAW_MANAGER->Render();
        //Render Object
        isWindowOpen = VULKAN_CORE->Render();

        SHARK_ENGINE->EndScene();
    }

    VULKAN_DRAW_MANAGER->ReleaseAll();
    VULKAN_TEXTURE_MANAGER->ReleaseTextures();
    VULKAN_CORE->Release();
    //appObj->Release();
}