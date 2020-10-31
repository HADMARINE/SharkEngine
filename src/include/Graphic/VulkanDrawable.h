//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_VULKANDRAWABLE_H
#define SHARKENGINE_VULKANDRAWABLE_H

#define NUMBER_OF_VIEWPORTS 1
#define NUMBER_OF_SCISSORS NUMBER_OF_VIEWPORTS

#pragma once
#include "IncludeVulkan.h"
#include "VulkanDescriptor.h"
#include "VulkanStructs.h"

class VulkanRenderer;
class VulkanDrawable : public VulkanDescriptor
{
public:
    VulkanDrawable(VulkanRenderer* parent = 0);
    ~VulkanDrawable();

    void CreateVertexBuffer(const void *vertexData, uint32_t dataSize, uint32_t dataStride, bool useTexture);
    void Prepare();
    void Update();
    void Render();

    void SetPipeline(VkPipeline* vulkanPipeline) { pipeline = vulkanPipeline; }
    VkPipeline* GetPipeline() { return pipeline; }

    void CreateUniformBuffer();
    void CreateDescriptorPool(bool useTexture);
    void CreateDescriptorResources();
    void CreateDescriptorSet(bool useTexture);
    void CreateDescriptorSetLayout(bool useTexture);
    void CreatePipelineLayout();

    void InitViewports(VkCommandBuffer* cmd);
    void InitScissors(VkCommandBuffer* cmd);

    void DestroyVertexBuffer();
    void DestroyCommandBuffer();
    void DestroySynchronizationObjects();
    void DestroyUniformBuffer();

    void SetTextures(TextureData* tex);
public:
    /***************DRAWABLE WRAPPERS***************/
    struct UniformData {
        VkBuffer						buffer;			// Buffer resource object
        VkDeviceMemory					memory;			// Buffer resourece object's allocated device memory
        VkDescriptorBufferInfo			bufferInfo;		// Buffer info that need to supplied into write descriptor set (VkWriteDescriptorSet)
        VkMemoryRequirements			memRqrmnt;		// Store the queried memory requirement of the uniform buffer
        std::vector<VkMappedMemoryRange>mappedRange;	// Metadata of memory mapped objects
        uint8_t*						pData;			// Host pointer containing the mapped device address which is used to write data into.
    } UniformData;

    // Structure storing vertex buffer metadata
    struct VertexBuffer {
        VkBuffer buf;
        VkDeviceMemory mem;
        VkDescriptorBufferInfo bufferInfo;
    } VertexBuffer;

    // Stores the vertex input rate
    VkVertexInputBindingDescription		viIpBinding;
    // Store metadata helpful in data interpretation
    VkVertexInputAttributeDescription	viIpAttribute[2];

private:
    void recordCommandBuffer(int currentImage, VkCommandBuffer* cmdDraw);

    std::vector<VkCommandBuffer> vecCmdDraw;			// Command buffer for drawing
    VkViewport viewport;
    VkRect2D   scissor;
    VkSemaphore presentCompleteSemaphore;
    VkSemaphore drawingCompleteSemaphore;
    TextureData* textures;

    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 Model;
    glm::mat4 MVP;

    VulkanRenderer* rendererObj;
    VkPipeline*		pipeline;
};


#endif//SHARKENGINE_VULKANDRAWABLE_H
