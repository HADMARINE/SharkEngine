//
// Created by HADMARINE on 2020/11/25.
//

#include "SharkVulkan.hpp"
#include "SharkVulkanDrawable.hpp"

std::vector<uint32_t> SharkEngine::Core::SharkVulkanDrawable::getVertices() {
    return this->vertices;
}

std::vector<uint32_t> SharkEngine::Core::SharkVulkanDrawable::getIndices() {
    return this->indices;
}

std::vector<VkDescriptorSet> SharkEngine::Core::SharkVulkanDrawable::getDescriptorSets() {
    return this->descriptorSets;
}

SharkEngine::Core::SharkVulkanDrawable::SharkVulkanDrawable(
        std::vector<uint32_t> vertices, const std::string &file) {

    this->vertices = vertices;

    uint32_t indexCount = 0;
    for (int i = 0; i < vertices.size() - 2; i++) {
        this->indices.push_back(indexCount);
        this->indices.push_back(indexCount + i + 1);
        this->indices.push_back(indexCount + i + 2);
    }

    int texWidth, texHeight, texChannels;

    stbi_uc *pixels = stbi_load(("../src/source/" + file).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        CLogger::Error("Failed to load texture image");
        throw std::runtime_error("Failed to load texture image");
    }

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    SHARK_API_CORE->CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(*SHARK_API_CORE->GetDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(*SHARK_API_CORE->GetDevice(), stagingBufferMemory);

    stbi_image_free(pixels);

    SHARK_API_CORE->CreateImage(texWidth, texHeight,
                VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, this->textureImage, this->textureImageMemory);

    SHARK_API_CORE->TransitionImageLayout(this->textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                                          VK_IMAGE_LAYOUT_UNDEFINED,
                                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    SHARK_API_CORE->CopyBufferToImage(stagingBuffer, this->textureImage,
                                      static_cast<uint32_t>(texWidth),
                                      static_cast<uint32_t>(texHeight));
    SHARK_API_CORE->TransitionImageLayout(this->textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(*SHARK_API_CORE->GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(*SHARK_API_CORE->GetDevice(), stagingBufferMemory, nullptr);

    this->textureImageView = SHARK_API_CORE->CreateImageView(this->textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                                                       VK_IMAGE_ASPECT_COLOR_BIT);




    std::vector<VkDescriptorSetLayout> layouts(SHARK_API_CORE->GetSwapChainImages().size(),
                                               SHARK_API_CORE->GetDescriptorSetLayout());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = SHARK_API_CORE->GetDescriptorPool();
    allocInfo.descriptorSetCount = static_cast<uint32_t>(SHARK_API_CORE->GetSwapChainImages().size());
    allocInfo.pSetLayouts = layouts.data();

    this->descriptorSets.resize(SHARK_API_CORE->GetSwapChainImages().size());
    if (vkAllocateDescriptorSets(*SHARK_API_CORE->GetDevice(), &allocInfo,
                                 this->descriptorSets.data()) != VK_SUCCESS) {
        CLogger::Error("Failed to allocate descriptor sets");
        throw std::runtime_error("Failed to allocate descriptor sets");
    }

    for (size_t i = 0; i < SHARK_API_CORE->GetSwapChainImages().size(); i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = SHARK_API_CORE->GetUniformBuffers()[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = textureImageView;
        imageInfo.sampler = SHARK_API_CORE->GetTextureSampler();

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(*SHARK_API_CORE->GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }

}

SharkEngine::Core::SharkVulkanDrawable::~SharkVulkanDrawable() {

}
