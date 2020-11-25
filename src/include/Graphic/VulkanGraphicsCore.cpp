////
//// Created by HADMARINE on 2020/11/03.
////
//
//#include "VulkanGraphicsCore.h"
//#include <stb_image.h>
//#include "VulkanDrawable.h"
//
//SharkEngine::TextureImageStruct SharkEngine::VulkanEngine::createTextureImage(const std::string location) {
//    {
//        int texWidth, texHeight, texChannels;
//        stbi_uc *pixels = stbi_load((std::string("../src/source/") + location).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
//        VkDeviceSize imageSize = texWidth * texHeight * 4;
//
//        if (!pixels) {
//            CLogger::Error("Failed to load texture image");
//            throw std::runtime_error("Failed to load texture image");
//        }
//
//        VkBuffer stagingBuffer;
//        VkDeviceMemory stagingBufferMemory;
//        createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
//
//        void *data;
//        vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
//        memcpy(data, pixels, static_cast<size_t>(imageSize));
//        vkUnmapMemory(device, stagingBufferMemory);
//
//        stbi_image_free(pixels);
//
//        VkImage fTextureImage;
//        VkDeviceMemory fTextureImageMemory;
//
//        createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
//                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, fTextureImage, fTextureImageMemory);
//
//        transitionImageLayout(fTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
//        copyBufferToImage(stagingBuffer, fTextureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
//        transitionImageLayout(fTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
//
//        vkDestroyBuffer(device, stagingBuffer, nullptr);
//        vkFreeMemory(device, stagingBufferMemory, nullptr);
//
//        TextureImageStruct texImgStructReturn{};
//
//        texImgStructReturn.image = reinterpret_cast<VkImage *>(fTextureImage);
//        texImgStructReturn.deviceMemory = reinterpret_cast<VkDeviceMemory *>(fTextureImageMemory);
//
//        return texImgStructReturn;
//    }
//}
//void SharkEngine::VulkanEngine::createCommandBuffers() {
//    commandBuffers.resize(swapChainFramebuffers.size());
//
//    VkCommandBufferAllocateInfo allocInfo{};
//    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//    allocInfo.commandPool = commandPool;
//    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();
//
//    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
//        CLogger::Error("Failed to allocate command buffers");
//        throw std::runtime_error("Failed to allocate command buffers");
//    }
//
//    for (size_t i = 0; i < commandBuffers.size(); i++) {
//        auto textureImageInitial = createTextureImage("texture2.jpg");
//        textureImage = *textureImageInitial.image;
//        textureImageMemory = *textureImageInitial.deviceMemory;
//        createTextureImageView();
//        for (size_t i = 0; i < swapChainImages.size(); i++) {
//            VkDescriptorBufferInfo bufferInfo{};
//            bufferInfo.buffer = uniformBuffers[i];
//            bufferInfo.offset = 0;
//            bufferInfo.range = sizeof(UniformBufferObject);
//
//            VkDescriptorImageInfo imageInfo{};
//            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//            imageInfo.imageView = textureImageView;
//            imageInfo.sampler = textureSampler;
//            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
//
//            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//            descriptorWrites[0].dstSet = descriptorSets[i];
//            descriptorWrites[0].dstBinding = 0;
//            descriptorWrites[0].dstArrayElement = 0;
//            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//            descriptorWrites[0].descriptorCount = 1;
//            descriptorWrites[0].pBufferInfo = &bufferInfo;
//
//            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//            descriptorWrites[1].dstSet = descriptorSets[i];
//            descriptorWrites[1].dstBinding = 1;
//            descriptorWrites[1].dstArrayElement = 0;
//            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//            descriptorWrites[1].descriptorCount = 1;
//            descriptorWrites[1].pImageInfo = &imageInfo;
//
//            vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
//        }
//
//
//        VkCommandBufferBeginInfo beginInfo{};
//        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//        beginInfo.flags = 0;
//        beginInfo.pInheritanceInfo = nullptr;
//
//        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
//            CLogger::Error("Failed to begin recording command buffer");
//            throw std::runtime_error("Failed to begin recording command buffer");
//        }
//
//        VkRenderPassBeginInfo renderPassInfo{};
//        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//        renderPassInfo.renderPass = renderPass;
//        renderPassInfo.framebuffer = swapChainFramebuffers[i];
//        renderPassInfo.renderArea.offset = {0, 0};
//        renderPassInfo.renderArea.extent = swapChainExtent;
//
//        std::array<VkClearValue, 2> clearValues{};
//        clearValues[0] = {0.0f, 0.0f, 0.0f, 1.0f};
//        clearValues[1] = {1.0f, 0};
//
//        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
//        renderPassInfo.pClearValues = clearValues.data();
//
//        vkCmdBeginRenderPass(commandBuffers[i],
//                             &renderPassInfo,
//                             VK_SUBPASS_CONTENTS_INLINE);
//
//        vkCmdBindPipeline(commandBuffers[i],
//                          VK_PIPELINE_BIND_POINT_GRAPHICS,
//                          graphicsPipeline);
//
//        VkBuffer vertexBuffers[] = {vertexBuffer};
//        VkDeviceSize offsets[] = {0};
//        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
//
//        vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
//
//        unsigned int indexCount = 0;
//
//        for (auto d : Drawable::drawables) {
//            vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
//                                    pipelineLayout, 0, 1, &d->descriptorSet.at(i),
//                                    0, nullptr);
//
//            vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, indexCount, 0);
//
//            indexCount += vertices.size();
//        }
//
//        vkCmdEndRenderPass(commandBuffers[i]);
//
//        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
//            CLogger::Error("Failed to record command buffer");
//            throw std::runtime_error("Failed to record command buffer");
//        }
//    }
//}
