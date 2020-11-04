//
// Created by EunwooSong on 2020-11-03.
//

#include "VulkanTextureManager.h"
#include "../../include/Engine/CoreDefine.h"
#include "VulkanGraphicsCore.h"
#include <map>

using namespace VulkanCore;

VulkanTextureManager::VulkanTextureManager() {
}
VulkanTextureManager::~VulkanTextureManager() {
}
VulkanTextureManager* VulkanTextureManager::Instance() {
    static VulkanTextureManager* instance = new VulkanTextureManager();
    return instance;
}
TextureImageStruct *VulkanTextureManager::GetTextureImage(std::string path) {
    if(textureImageList[path] == nullptr) {
        textureImageList[path] =  new TextureImageStruct();

        TextureImageStruct tmp = VULKAN_CORE->createTextureImage(path);

        textureImageList[path]->image = tmp.image;
        textureImageList[path]->deviceMemory = tmp.deviceMemory;
    }

    return textureImageList[path];
}
void VulkanTextureManager::ReleaseTextures() {
    for(auto iter : textureImageList) {
        vkDestroyImage(VULKAN_CORE->GetDevice(), iter.second->image, nullptr);
        vkFreeMemory(VULKAN_CORE->GetDevice(), iter.second->deviceMemory, nullptr);
        SAFE_DELETE(iter.second);
    }
}
