//
// Created by EunwooSong on 2020-11-03.
//

#ifndef SHARKENGINE_VULKANDRAWABLE_H
#define SHARKENGINE_VULKANDRAWABLE_H

#include "VulkanGraphicCore.h"

class VulkanDrawable {
public:
public:
    VulkanDrawable(VulkanCore::VulkanEngine *core) : Core(core), isReady(false), isDestroy(false) { vertexData.resize(0); }
    VulkanDrawable(VulkanCore::VulkanEngine *core, VulkanCore::TextureImageStruct *texture, const std::vector<VulkanCore::Vertex> &vertices, const std::vector<uint16_t> &indices) : Core(core), isReady(false), isDestroy(false), vertexData(vertices), indices(indices) { SetTexture(texture); }
    ~VulkanDrawable() {}

    void Destroy() {
        isDestroy = true;
    }
    bool GetIsDestroy() {
        return isDestroy;
    }

    void SetTexture(VulkanCore::TextureImageStruct *texture) {
        image = &texture->image;
        mem = &texture->deviceMemory;

        if (isReady)
            cleanup();

        isReady = false;
    }

    void SetVertexData(const std::vector<VulkanCore::Vertex> &vertices) {
        vertexData = vertices;
    }

    void InitObject() {
        createTextureImageView();
        createTextureSampler();
        createVertexBuffer();
        createIndexBuffer();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        createCommandBuffers();
        createSyncObjects();

        isReady = true;
    }

    //Main Func
    void createTextureImageView() {
        view = Core->createImageView(*image, VK_FORMAT_R8G8B8A8_SRGB,
                                     VK_IMAGE_ASPECT_COLOR_BIT);
    }

    void createTextureSampler() {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        // if device not supported, set antisotropyEnable to VK_FALSE, maxAntisotropy to 1.0f
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = 16.0f;

        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(Core->GetDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
            CLogger::Error("Failed to create texture sampler");
            throw std::runtime_error("Failed to create texture sampler");
        }
    }
    void createVertexBuffer() {
        VkDeviceSize bufferSize = sizeof(vertexData[0]) * vertexData.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        Core->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           stagingBuffer, stagingBufferMemory);

        void *data;
        vkMapMemory(Core->GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertexData.data(), (size_t) bufferSize);
        vkUnmapMemory(Core->GetDevice(), stagingBufferMemory);

        Core->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

        Core->copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
        vkDestroyBuffer(Core->GetDevice(), stagingBuffer, nullptr);
        vkFreeMemory(Core->GetDevice(), stagingBufferMemory, nullptr);
    }
    void createIndexBuffer() {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        Core->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           stagingBuffer, stagingBufferMemory);

        void *data;
        vkMapMemory(Core->GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t) bufferSize);
        vkUnmapMemory(Core->GetDevice(), stagingBufferMemory);

        Core->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

        Core->copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(Core->GetDevice(), stagingBuffer, nullptr);
        vkFreeMemory(Core->GetDevice(), stagingBufferMemory, nullptr);
    }
    void createUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(VulkanCore::UniformBufferObject);
        uniformBuffers.resize(Core->getSwapChainImageCount());
        uniformBuffersMemory.resize(Core->getSwapChainImageCount());

        for (size_t i = 0; i < Core->getSwapChainImageCount(); i++) {
            Core->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               uniformBuffers[i], uniformBuffersMemory[i]);
        }
    }
    void createDescriptorPool() {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(Core->getSwapChainImageCount());
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(Core->getSwapChainImageCount());

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(Core->getSwapChainImageCount());

        if (vkCreateDescriptorPool(Core->GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            CLogger::Error("Failed to create descriptor pool");
            throw std::runtime_error("Failed to create descriptor pool");
        }
    }
    void createDescriptorSets() {
        std::vector<VkDescriptorSetLayout> layouts(Core->getSwapChainImageCount(), Core->GetDescriptorSetLayout());
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(Core->getSwapChainImageCount());
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(Core->getSwapChainImageCount());

        if (vkAllocateDescriptorSets(Core->GetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            CLogger::Error("Failed to allocate descriptor sets");
            throw std::runtime_error("Failed to allocate descriptor sets");
        }

        for (size_t i = 0; i < Core->getSwapChainImageCount(); i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(VulkanCore::UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = view;
            imageInfo.sampler = sampler;
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

            vkUpdateDescriptorSets(Core->GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }
    void createCommandBuffers() {
        commandBuffers.resize(Core->getSwapChainImageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = Core->GetCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

        if (vkAllocateCommandBuffers(Core->GetDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            CLogger::Error("Failed to allocate command buffers");
            throw std::runtime_error("Failed to allocate command buffers");
        }

        for (size_t i = 0; i < commandBuffers.size(); i++) {
            for (size_t i = 0; i < Core->getSwapChainImageCount(); i++) {
                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = uniformBuffers[i];
                bufferInfo.offset = 0;
                bufferInfo.range = sizeof(VulkanCore::UniformBufferObject);

                VkDescriptorImageInfo imageInfo{};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = view;
                imageInfo.sampler = sampler;
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

                vkUpdateDescriptorSets(Core->GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
            }

            //            VkCommandBufferBeginInfo beginInfo{};
            //            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            //            beginInfo.flags = 0;
            //            beginInfo.pInheritanceInfo = nullptr;
            //
            //            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            //                CLogger::Error("Failed to begin recording command buffer");
            //                throw std::runtime_error("Failed to begin recording command buffer");
            //            }
            //
            //            VkRenderPassBeginInfo renderPassInfo{};
            //            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            //            renderPassInfo.renderPass = Core->GetRenderPass();
            //            renderPassInfo.framebuffer = Core->GetSwapChainFrameBuffer()[i];
            //            renderPassInfo.renderArea.offset = {0, 0};
            //            renderPassInfo.renderArea.extent = Core->GetSwapChainExtent();
            //
            //            std::array<VkClearValue, 2> clearValues{};
            //            clearValues[0] = {0.0f, 0.0f, 0.0f, 0.0f};
            //            clearValues[1] = {1.0f, 0};
            //
            //            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            //            renderPassInfo.pClearValues = clearValues.data();
            //
            //            vkCmdBeginRenderPass(commandBuffers[i],
            //                                 &renderPassInfo,
            //                                 VK_SUBPASS_CONTENTS_INLINE);
            //
            //            vkCmdBindPipeline(commandBuffers[i],
            //                              VK_PIPELINE_BIND_POINT_GRAPHICS,
            //                              Core->GetGraphicPipeline());
            //
            //            VkBuffer vertexBuffers[] = {vertexBuffer};
            //            VkDeviceSize offsets[] = {0};
            //            vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
            //
            //            vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
            //
            //            vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
            //                                    Core->GetPipelineLayout(), 0, 1, &descriptorSets[i], 0, nullptr);
            //
            //            vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
            //
            //            vkCmdEndRenderPass(commandBuffers[i]);
            //
            //            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            //                CLogger::Error("Failed to record command buffer");
            //                throw std::runtime_error("Failed to record command buffer");
            //            }
        }
    }

    void StartRenderPass() {
        for (size_t i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = 0;
            beginInfo.pInheritanceInfo = nullptr;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                CLogger::Error("Failed to begin recording command buffer");
                throw std::runtime_error("Failed to begin recording command buffer");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = Core->GetRenderPass();
            renderPassInfo.framebuffer = Core->GetSwapChainFrameBuffer()[i];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = Core->GetSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0] = {0.0f, 0.0f, 0.0f, 0.0f};
            clearValues[1] = {1.0f, 0};

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i],
                                 &renderPassInfo,
                                 VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffers[i],
                              VK_PIPELINE_BIND_POINT_GRAPHICS,
                              Core->GetGraphicPipeline());

            VkBuffer vertexBuffers[] = {vertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);

            vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    Core->GetPipelineLayout(), 0, 1, &descriptorSets[i], 0, nullptr);

            vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
        }
    }

    void drawIndexed() {
        for (size_t i = 0; i < commandBuffers.size(); i++) {
            vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
        }
    }

    void endRenderPass() {
        for (size_t i = 0; i < commandBuffers.size(); i++) {
            vkCmdEndRenderPass(commandBuffers[i]);

            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                CLogger::Error("Failed to record command buffer");
                throw std::runtime_error("Failed to record command buffer");
            }
        }
    }

    void createSyncObjects() {
        imageAvailableSemaphores.resize(GlobalPreferences::MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(GlobalPreferences::MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(GlobalPreferences::MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(Core->getSwapChainImageCount(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < GlobalPreferences::MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(Core->GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(Core->GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(Core->GetDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

                CLogger::Error("Failed to create semaphores for a frame");
                throw std::runtime_error("Failed to create semaphores for a frame");
            }
        }
    }

    //Util Func
    VkImageView createImageView(VkImage image, VkFormat format,
                                VkImageAspectFlags aspectFlags) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(Core->GetDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            CLogger::Error("Failed to create texture image view");
            throw std::runtime_error("Failed to create texture image view");
        }

        return imageView;
    }


    void drawFrame() {
        vkWaitForFences(Core->GetDevice(), 1, &inFlightFences[Core->GetCurrentFrame()], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(Core->GetDevice(), Core->GetSwapChain(), UINT64_MAX, imageAvailableSemaphores[Core->GetCurrentFrame()], VK_NULL_HANDLE, &imageIndex);

        updateUniformBuffer(imageIndex);

        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(Core->GetDevice(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        imagesInFlight[imageIndex] = inFlightFences[Core->GetCurrentFrame()];

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[Core->GetCurrentFrame()]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[Core->GetCurrentFrame()]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(Core->GetDevice(), 1, &inFlightFences[Core->GetCurrentFrame()]);
        //
        if (vkQueueSubmit(Core->GetGraphicQueue(), 1, &submitInfo, inFlightFences[Core->GetCurrentFrame()]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {Core->GetSwapChain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(Core->GetPresentQueue(), &presentInfo);

        if (result != VK_SUCCESS) {
            CLogger::Error("Failed to present swap chain image");
            throw std::runtime_error("Failed to present swap chain image");
        }

        vkQueueWaitIdle(Core->GetPresentQueue());

        //        assert(result);
        //
        //        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        //            framebufferResized = false;
        //            recreateSwapChain();
        //        } else if (result != VK_SUCCESS) {
        //            throw std::runtime_error("failed to present swap chain image!");
        //        }
        //
        //        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void updateUniformBuffer(uint32_t currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        VulkanCore::UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), Core->GetSwapChainExtent().width / (float) Core->GetSwapChainExtent().height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        void *data;
        vkMapMemory(Core->GetDevice(), uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(Core->GetDevice(), uniformBuffersMemory[currentImage]);
    }

    void cleanupSwapChain() {
        for (size_t i = 0; i < Core->getSwapChainImageCount(); i++) {
            vkDestroyBuffer(Core->GetDevice(), uniformBuffers[i], nullptr);
            vkFreeMemory(Core->GetDevice(), uniformBuffersMemory[i], nullptr);
        }

        vkDestroyDescriptorPool(Core->GetDevice(), descriptorPool, nullptr);
    }

    void cleanup() {
        vkDestroySampler(Core->GetDevice(), sampler, nullptr);

        vkDestroyImageView(Core->GetDevice(), view, nullptr);

        //        Free TextureStruct array
        //        for (TextureImageStruct textureImageStruct : *textureImageStructs) {
        //            vkDestroyImage(device, *textureImageStruct.image, nullptr);
        //            vkFreeMemory(device, *textureImageStruct.deviceMemory, nullptr);
        //        }

        vkDestroyDescriptorSetLayout(Core->GetDevice(), descriptorSetLayout, nullptr);

        vkDestroyBuffer(Core->GetDevice(), indexBuffer, nullptr);
        vkFreeMemory(Core->GetDevice(), indexBufferMemory, nullptr);

        vkDestroyBuffer(Core->GetDevice(), vertexBuffer, nullptr);
        vkFreeMemory(Core->GetDevice(), vertexBufferMemory, nullptr);

        for (size_t i = 0; i < GlobalPreferences::MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(Core->GetDevice(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(Core->GetDevice(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(Core->GetDevice(), inFlightFences[i], nullptr);
        }
    }

public:
    // Pipeline layout object
    VkPipelineLayout pipelineLayout;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;// Command buffer for drawing

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;

    //Matrix
    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 Model;
    glm::mat4 MVP;

    //Uniform Buffer
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    //VertexData;
    std::vector<VulkanCore::Vertex> vertexData;

    //Image Data
    VkSampler sampler;
    VkImage *image;
    VkImageLayout imageLayout;
    VkMemoryAllocateInfo memoryAlloc;
    VkDeviceMemory *mem;
    VkImageView view;
    uint32_t mipMapLevels;
    uint32_t layerCount;
    uint32_t textureWidth, textureHeight;
    VkDescriptorImageInfo descsImgInfo;

    //Index Data
    std::vector<uint16_t> indices;

    VulkanCore::VulkanEngine *Core;

    bool isReady;
    bool isDestroy;
};
#endif//SHARKENGINE_VULKANDRAWABLE_H
