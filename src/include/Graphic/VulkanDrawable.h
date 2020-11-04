//
// Created by HADMARINE on 2020-11-04.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define STB_IMAGE_IMPLEMENTATION

#include "VulkanGraphicsCore.h"
#include <vulkan/vulkan.h>


namespace SharkEngine {

    class Drawable {
    public:
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        std::vector<VkDescriptorSet> descriptorSet;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        struct DrawableVertexValue {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
        };


        static std::vector<Drawable*> drawables;

        static DrawableVertexValue returnVertexValue() {
                DrawableVertexValue dv{};
                return dv;
        };

        Drawable(TextureImageStruct* image, const std::vector<Vertex> &vertices, std::string textureName) {
            this->vertices = vertices;

            textureImage = image->image;
            textureImageMemory = image->deviceMemory;

            for (int i = 0; i < vertices.size() - 2; i++) {
                this->indices.push_back(indexCount);
                this->indices.push_back(indexCount + i + 1);
                this->indices.push_back(indexCount + i + 2);
            }
            indexCount += vertices.size();
        }
    };
};