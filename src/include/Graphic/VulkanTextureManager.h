//
// Created by EunwooSong on 2020-11-03.
//

#ifndef SHARKENGINE_VULKANTEXTUREMANAGER_H
#define SHARKENGINE_VULKANTEXTUREMANAGER_H

#include <string>
#include <map>

#define VULKAN_TEXTURE_MANAGER VulkanTextureManager::Instance()

namespace VulkanCore {
    struct TextureImageStruct;

    class VulkanTextureManager {
    public:
        VulkanTextureManager();
        ~VulkanTextureManager();

        static VulkanTextureManager* Instance();

        TextureImageStruct* GetTextureImage(std::string path);

        void ReleaseTextures();

    private:
        std::map<std::string, TextureImageStruct*> textureImageList;
    };

}

#endif//SHARKENGINE_VULKANTEXTUREMANAGER_H
