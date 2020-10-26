//
// Created by HADMARINE on 2020/10/24.
//

//#include "VulkanEngine.hpp"
//
//using namespace SharkEngine;

//void VulkanEngine::createTextureImage(const char* filename) {
//    int texWidth, texHeight, texChannels;
//    stbi_uc *pixels = stbi_load("../src/source/texture.jpg" , &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
//    VkDeviceSize imageSize = texWidth * texHeight * 4;
//
//    if (!pixels) {
//        CLogger::Error("Failed to load texture image");
//        throw std::runtime_error("Failed to load texture image");
//    }
//
//    VkBuffer stagingBuffer;
//    VkDeviceMemory stagingBufferMemory;
//    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
//
//    void *data;
//    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
//    memcpy(data, pixels, static_cast<size_t>(imageSize));
//    vkUnmapMemory(device, stagingBufferMemory);
//
//    stbi_image_free(pixels);
//
//    createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
//                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);
//
//    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
//    copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
//    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
//
//    vkDestroyBuffer(device, stagingBuffer, nullptr);
//    vkFreeMemory(device, stagingBufferMemory, nullptr);
//}