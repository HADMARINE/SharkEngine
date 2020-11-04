//
// Created by EunwooSong on 2020-11-03.
//

#include "../../include/Graphic/VulkanDrawableManager.h"
#include "../../include/Graphic/VulkanDrawable.h"
#include "../../include/Graphic/VulkanGraphicsCore.h"

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
    for (size_t i = 0; i < VulkanCore::VULKAN_CORE->GetCommandBuffer().size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(VulkanCore::VULKAN_CORE->GetCommandBuffer()[i], &beginInfo) != VK_SUCCESS) {
            CLogger::Error("Failed to begin recording command buffer");
            throw std::runtime_error("Failed to begin recording command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = VulkanCore::VULKAN_CORE->GetRenderPass();
        renderPassInfo.framebuffer = VulkanCore::VULKAN_CORE->GetSwapChainFrameBuffer()[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = VulkanCore::VULKAN_CORE->GetSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0] = {0.0f, 0.0f, 0.0f, 0.0f};
        clearValues[1] = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(VulkanCore::VULKAN_CORE->GetCommandBuffer()[i],
                             &renderPassInfo,
                             VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(VulkanCore::VULKAN_CORE->GetCommandBuffer()[i],
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          VulkanCore::VULKAN_CORE->GetGraphicPipeline());

        //Draw Indexed!! -----------------------
        for(auto iter : drawablesList) {
            iter->drawIndexed(VulkanCore::VULKAN_CORE->GetCommandBuffer()[i], i);
        }
        //--------------------------------------

        vkCmdEndRenderPass(VulkanCore::VULKAN_CORE->GetCommandBuffer()[i]);

        if (vkEndCommandBuffer(VulkanCore::VULKAN_CORE->GetCommandBuffer()[i]) != VK_SUCCESS) {
            CLogger::Error("Failed to record command buffer");
            throw std::runtime_error("Failed to record command buffer");
        }
    }

//    for(auto iter : drawablesList)
//        iter->StartRenderPass();

//    for(auto iter : drawablesList)
//        iter->drawIndexed();
//
//    for(auto iter : drawablesList)
//        iter->endRenderPass();
//
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
    }
}
