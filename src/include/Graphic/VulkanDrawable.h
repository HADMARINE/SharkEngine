////
//// Created by HADMARINE on 2020-11-04.
////
//
//#pragma once
//#include "VulkanGraphicsCore.h"
//#include "VulkanTextureManager.h"
//
//
//namespace SharkEngine {
//    VkImageView createImageView(VkImage image, VkFormat format,
//                                VkImageAspectFlags aspectFlags) {
//        VkImageViewCreateInfo viewInfo{};
//        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
//        viewInfo.image = image;
//        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
//        viewInfo.format = format;
//        viewInfo.subresourceRange.aspectMask = aspectFlags;
//        viewInfo.subresourceRange.baseMipLevel = 0;
//        viewInfo.subresourceRange.levelCount = 1;
//        viewInfo.subresourceRange.baseArrayLayer = 0;
//        viewInfo.subresourceRange.layerCount = 1;
//
//        VkImageView imageView;
//        if (vkCreateImageView(reinterpret_cast<VkDevice>(VULKAN_CORE), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
//            CLogger::Error("Failed to create texture image view");
//            throw std::runtime_error("Failed to create texture image view");
//        }
//
//        return imageView;
//    }
//
//    class Drawable {
//    public:
//        VkImage textureImage;
//        VkDeviceMemory textureImageMemory;
//        VkImageView textureImageView;
//        std::vector<VkDescriptorSet> descriptorSet;
//        std::vector<Vertex> vertices;
//        std::vector<uint32_t> indices;
//
//        struct DrawableVertexValue {
//            std::vector<Vertex> vertices;
//            std::vector<uint32_t> indices;
//        };
//
//
//        static std::vector<Drawable *> drawables;
//        static uint32_t indexCount;
//
//        static DrawableVertexValue returnVertexValue() {
//            DrawableVertexValue dv{};
//            return dv;
//        };
//
//        Drawable(std::string uri, const std::vector<Vertex> &vertices) {
//            this->vertices = vertices;
//
//            TextureImageStruct *texture = SharkEngine::VULKAN_TEXTURE_MANAGER->GetTextureImage(uri);
//            for (int i = 0; i < vertices.size() - 2; i++) {
//                this->indices.push_back(indexCount);
//                this->indices.push_back(indexCount + i + 1);
//                this->indices.push_back(indexCount + i + 2);
//            }
//            indexCount += vertices.size();
//
//            this->textureImage = reinterpret_cast<VkImage>(texture->image);
//            this->textureImageMemory = reinterpret_cast<VkDeviceMemory>(texture->deviceMemory);
//            //            this->textureImageView =
//        }
//    };
//
//    uint32_t Drawable::indexCount = 0;
//    std::vector<Drawable *> drawables = {};
//};// namespace SharkEngine