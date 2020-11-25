//
// Created by HADMARINE on 2020/09/20.
//

//#include "include/Graphic/VulkanApplication.h"
#include "InGame/Scenes/TestScene.h"
#include "include/Engine/SharkSystem.hpp"
#include "include/Graphic/SharkVulkan.hpp"
//#include "include/Graphic/VulkanDrawable.h"
//#include "include/Graphic/VulkanDrawableManager.h"
//#include "include/Graphic/VulkanGraphicsCore.h"
//#include "include/Graphic/VulkanTextureManager.h"
//#include "include/Engine/UniformBufferCamera/Camera.hpp"

using namespace SharkEngine::Core;


std::unique_ptr<SharkSystem> SharkSystem::instance;
std::once_flag SharkSystem::onlyOnce;

int main() {
    using namespace SharkEngine;
    //    VulkanApplication* appObj = VulkanApplication::GetInstance();
    //
    //    appObj->Initialize();
    //    appObj->Prepare();

    //ROLLBACK_GAME_PROJECT

    SHARK_ENGINE->InitializeEngine();

    SHARK_API_CORE->Run();


//    VulkanDrawable *tmp = reinterpret_cast<VulkanDrawable *>(new Drawable("texture.jpg", std::vector<Vertex>{
//                                                                                                 {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
//                                                                                                 {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
//                                                                                                 {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
//                                                                                                 {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}}));
//
//    VulkanDrawable *tmp_2 = reinterpret_cast<VulkanDrawable *>(new Drawable("viking_room.png", std::vector<Vertex>{
//                                                                                                       {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
//                                                                                                       {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
//                                                                                                       {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
//                                                                                                       {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}}));


//    VULKAN_DRAW_MANAGER->AddVulkanDrawable(tmp);
//    VULKAN_DRAW_MANAGER->AddVulkanDrawable(tmp_2);

    SHARK_ENGINE->GetSceneManager()->ChangeScene(new TestScene());

    bool isWindowOpen = true;
//    while (isWindowOpen) {
//        VULKAN_DRAW_MANAGER->CheckInit();
//
//        SHARK_ENGINE->Start();
//        //appObj->Update();
//        SHARK_ENGINE->Update();
//        SHARK_ENGINE->LateUpdate();
//        SHARK_ENGINE->Render();
//
//        VULKAN_DRAW_MANAGER->Render();
//        //Render Object
//        isWindowOpen = VULKAN_CORE->Render();
//
//        SHARK_ENGINE->EndScene();
//    }

///
///
///

//    VULKAN_DRAW_MANAGER->ReleaseAll();
//    VULKAN_TEXTURE_MANAGER->ReleaseTextures();
//    VULKAN_CORE->Release();
    //appObj->Release();
}