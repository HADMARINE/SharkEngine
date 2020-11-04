//
// Created by EunwooSong on 2020-11-03.
//

#include "../../include/Graphic/VulkanDrawableManager.h"
#include "../../include/Graphic/VulkanDrawable.h"

VulkanDrawableManager *VulkanDrawableManager::Instance() {
    static VulkanDrawableManager* instance = new VulkanDrawableManager();
    return instance;
}
void VulkanDrawableManager::CheckInit() {
    for(auto iter : drawablesList)
        if(!iter->isReady)
            iter->InitObject();
}
void VulkanDrawableManager::Render() {
    //Create Command Buffer

    //

    for(auto iter : drawablesList)
        iter->StartRenderPass();

    for(auto iter : drawablesList)
        iter->drawIndexed();

    for(auto iter : drawablesList)
        iter->endRenderPass();

    for(auto iter : drawablesList)
        iter->drawFrame();
}
void VulkanDrawableManager::EndScene() {
    drawablesList.erase(remove_if(drawablesList.begin(), drawablesList.end(),[&](VulkanDrawable* iter) {
        bool remove = iter->GetIsDestroy();
        if(remove){
            iter->cleanup();
            delete iter;
        }
        return remove;
    }), drawablesList.end());
}
void VulkanDrawableManager::ReleaseAll() {
    for(auto iter : drawablesList) {
        iter->cleanup();
        delete iter;
    }
}
void VulkanDrawableManager::AddVulkanDrawable(VulkanDrawable *drawIter) {
    drawablesList.push_back(drawIter);
}
void VulkanDrawableManager::DestroyVulkanDrawable(VulkanDrawable *drawIter) {
    drawIter->Destroy();
}
void VulkanDrawableManager::ReCreateSwapChain() {
    for(auto iter : drawablesList) {
        iter->cleanupSwapChain();

        iter->createUniformBuffers();
        iter->createDescriptorPool();
        iter->createDescriptorSets();
        iter->createCommandBuffers();
    }
}
