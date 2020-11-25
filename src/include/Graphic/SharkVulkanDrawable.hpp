//
// Created by HADMARINE on 2020/11/25.
//

#ifndef SHARKENGINE_SHARKVULKANDRAWABLE_HPP
#define SHARKENGINE_SHARKVULKANDRAWABLE_HPP

#include "stdafx.hpp"
#include "IncludeVulkan.hpp"

namespace SharkEngine::Core{
    class SharkVulkanDrawable {
    public:
        SharkVulkanDrawable(const std::vector<Vertex>& vertices, const std::string &file);
        ~SharkVulkanDrawable();
        std::vector<Vertex> getVertices();
        std::vector<uint32_t> getIndices();
        std::vector<VkDescriptorSet> getDescriptorSets();
        
        static uint32_t indexCount;
    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<VkDescriptorSet> descriptorSets;
        VkImage textureImage{};
        VkDeviceMemory textureImageMemory{};
        VkImageView textureImageView;
    };
}


#endif//SHARKENGINE_SHARKVULKANDRAWABLE_HPP
