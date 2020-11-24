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
        SharkVulkanDrawable(std::vector<uint32_t> vertices, const std::string &file);
        ~SharkVulkanDrawable();
        std::vector<uint32_t> getVertices();
        std::vector<uint32_t> getIndices();
        std::vector<VkDescriptorSet> getDescriptorSets();
    private:
        std::vector<uint32_t> vertices;
        std::vector<uint32_t> indices;
        std::vector<VkDescriptorSet> descriptorSets;
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
    };
}


#endif//SHARKENGINE_SHARKVULKANDRAWABLE_HPP
