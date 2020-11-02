//
// Created by EunwooSong on 2020-10-27.
//

#include "../../include/Graphic/VulkanDescriptor.h"
#include "../../include/Graphic/VulkanApplication.h"
#include "../../include/Graphic/VulkanDevice.h"

VulkanDescriptor::VulkanDescriptor() {
    deviceObj = VulkanApplication::GetInstance()->deviceObj;
}
VulkanDescriptor::~VulkanDescriptor() {
}
void VulkanDescriptor::CreateDescriptor(bool useTexture) {
    CreateDescriptorResources();

    // Create the descriptor pool and use it for descriptor set allocation
    CreateDescriptorPool(useTexture);

    CreateDescriptorSet(useTexture);
}
void VulkanDescriptor::DestroyDescriptor() {
    DestroyDescriptorLayout();
    DestroyPipelineLayouts();
    DestroyDescriptorSet();
    DestroyDescriptorPool();
}
void VulkanDescriptor::DestroyDescriptorLayout() {
    for (int i = 0; i < descLayout.size(); i++) {
        vkDestroyDescriptorSetLayout(deviceObj->device, descLayout[i], NULL);
    }
    descLayout.clear();
}
void VulkanDescriptor::DestroyDescriptorPool() {
    vkDestroyPipelineLayout(deviceObj->device, pipelineLayout, NULL);
}
void VulkanDescriptor::DestroyDescriptorSet() {
    vkDestroyDescriptorPool(deviceObj->device, descriptorPool, NULL);
}
void VulkanDescriptor::DestroyPipelineLayouts() {
    vkFreeDescriptorSets(deviceObj->device, descriptorPool, (uint32_t)descriptorSet.size(), &descriptorSet[0]);
}
