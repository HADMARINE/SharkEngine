//
// Created by EunwooSong on 2020-10-27.
//

#include "../../include/Graphic/VulkanDrawable.h"
#include "../../include/Graphic/VulkanApplication.h"
#include "../../include/Engine/UniformBufferCamera/Camera.hpp"

VulkanDrawable::VulkanDrawable(VulkanRenderer *parent) {
    memset(&UniformData, 0, sizeof(UniformData));
    memset(&VertexBuffer, 0, sizeof(VertexBuffer));
    rendererObj = parent;

    VkSemaphoreCreateInfo presentCompleteSemaphoreCreateInfo;
    presentCompleteSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    presentCompleteSemaphoreCreateInfo.pNext = NULL;
    presentCompleteSemaphoreCreateInfo.flags = 0;

    VkSemaphoreCreateInfo drawingCompleteSemaphoreCreateInfo;
    drawingCompleteSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    drawingCompleteSemaphoreCreateInfo.pNext = NULL;
    drawingCompleteSemaphoreCreateInfo.flags = 0;

    VulkanDevice* deviceObj = VulkanApplication::GetInstance()->deviceObj;

    vkCreateSemaphore(deviceObj->device, &presentCompleteSemaphoreCreateInfo, NULL, &presentCompleteSemaphore);
    vkCreateSemaphore(deviceObj->device, &drawingCompleteSemaphoreCreateInfo, NULL, &drawingCompleteSemaphore);
}
VulkanDrawable::~VulkanDrawable() {
}
void VulkanDrawable::CreateVertexBuffer(const void *vertexData, uint32_t dataSize, uint32_t dataStride, bool useTexture) {

    VulkanApplication* appObj	= VulkanApplication::GetInstance();
    VulkanDevice* deviceObj		= appObj->deviceObj;

    VkResult  result;
    bool  pass;

    // Create the Buffer resourece metadata information
    VkBufferCreateInfo bufInfo		= {};
    bufInfo.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.pNext					= NULL;
    bufInfo.usage					= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufInfo.size					= dataSize;
    bufInfo.queueFamilyIndexCount	= 0;
    bufInfo.pQueueFamilyIndices	= NULL;
    bufInfo.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
    bufInfo.flags					= 0;

    result = vkCreateBuffer(deviceObj->device, &bufInfo, NULL, &VertexBuffer.buf);
    assert(result == VK_SUCCESS);

    VkMemoryRequirements memRequirement;
    vkGetBufferMemoryRequirements(deviceObj->device, VertexBuffer.buf, &memRequirement);

    // Create memory allocation metadata information
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext				= nullptr;
    allocInfo.memoryTypeIndex	= 0;
    allocInfo.allocationSize	= memRequirement.size;

    // Get the compatible type of memory
    pass = deviceObj->MemoryTypeFromProperties(memRequirement.memoryTypeBits,
                                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &allocInfo.memoryTypeIndex);
    assert(pass);

    // Allocate the physical backing for buffer resource
    result = vkAllocateMemory(deviceObj->device, &allocInfo, NULL, &(VertexBuffer.mem));
    assert(result == VK_SUCCESS);
    VertexBuffer.bufferInfo.range	= memRequirement.size;
    VertexBuffer.bufferInfo.offset	= 0;

    // Map the physical device memory region to the host
    uint8_t *pData;
    result = vkMapMemory(deviceObj->device, VertexBuffer.mem, 0, memRequirement.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);

    // Copy the data in the mapped memory
    memcpy(pData, vertexData, dataSize);

    // Unmap the device memory
    vkUnmapMemory(deviceObj->device, VertexBuffer.mem);

    // Bind the allocated buffer resource to the device memory
    result = vkBindBufferMemory(deviceObj->device, VertexBuffer.buf, VertexBuffer.mem, 0);
    assert(result == VK_SUCCESS);

    viIpBinding.binding		= 0;
    viIpBinding.inputRate		= VK_VERTEX_INPUT_RATE_VERTEX;
    viIpBinding.stride			= dataStride;

    viIpAttribute[0].binding	= 0;
    viIpAttribute[0].location	= 0;
    viIpAttribute[0].format		= VK_FORMAT_R32G32B32A32_SFLOAT;
    viIpAttribute[0].offset		= 0;
    viIpAttribute[1].binding	= 0;
    viIpAttribute[1].location	= 1;
    viIpAttribute[1].format		= useTexture ? VK_FORMAT_R32G32_SFLOAT : VK_FORMAT_R32G32B32A32_SFLOAT;
    viIpAttribute[1].offset		= 16; // After, 4 components - RGBA  each of 4 bytes(32bits)
}
void VulkanDrawable::Prepare() {
    VulkanDevice* deviceObj = rendererObj->GetDevice();
    vecCmdDraw.resize(rendererObj->GetSwapChain()->scPublicVars.colorBuffer.size());

    for (int i = 0; i < rendererObj->GetSwapChain()->scPublicVars.colorBuffer.size(); i++){
        CommandBufferMgr::allocCommandBuffer(&deviceObj->device, *rendererObj->GetCommandPool(), &vecCmdDraw[i]);
        CommandBufferMgr::beginCommandBuffer(vecCmdDraw[i]);

        // Create the render pass instance
        recordCommandBuffer(i, &vecCmdDraw[i]);

        // Finish the command buffer recording
        CommandBufferMgr::endCommandBuffer(vecCmdDraw[i]);
    }
}
void VulkanDrawable::Update() {
    VulkanDevice* deviceObj = rendererObj->GetDevice();

    auto camProp = SharkEngine::Camera::getCurrentCamera()->getCameraProperties();

    Projection = glm::perspective(glm::radians(camProp->fieldOfView), 1.0f, 0.1f, 100.0f);
    View = glm::lookAt(
            camProp->eye,		    // Camera is in World Space
            camProp->center,		// and looks at the origin
            camProp->upVector		    // Head is up
    );
    Model = camProp->model;
    static float rot = 0;
    rot += .0005f;
    Model = glm::rotate(Model, rot, glm::vec3(0.0, 1.0, 0.0))
            * glm::rotate(Model, rot, glm::vec3(1.0, 1.0, 1.0));

    glm::mat4 MVP = Projection * View * Model;

    VkResult res = vkInvalidateMappedMemoryRanges(deviceObj->device, 1, &UniformData.mappedRange[0]);

    memcpy(UniformData.pData, &MVP, sizeof(MVP));

    res = vkFlushMappedMemoryRanges(deviceObj->device, 1, &UniformData.mappedRange[0]);
}
void VulkanDrawable::Render() {
    VulkanDevice* deviceObj			= rendererObj->GetDevice();
    VulkanSwapChain* swapChainObj	= rendererObj->GetSwapChain();

    uint32_t& currentColorImage		= swapChainObj->scPublicVars.currentColorBuffer;
    VkSwapchainKHR& swapChain		= swapChainObj->scPublicVars.swapChain;

    VkFence nullFence = VK_NULL_HANDLE;

    // Get the index of the next available swapchain image:
    VkResult result = swapChainObj->fpAcquireNextImageKHR(deviceObj->device, swapChain,
                                                          UINT64_MAX, presentCompleteSemaphore, VK_NULL_HANDLE, &currentColorImage);

    VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext				= NULL;
    submitInfo.waitSemaphoreCount	= 1;
    submitInfo.pWaitSemaphores		= &presentCompleteSemaphore;
    submitInfo.pWaitDstStageMask	= &submitPipelineStages;
    submitInfo.commandBufferCount	= (uint32_t)sizeof(&vecCmdDraw[currentColorImage]) / sizeof(VkCommandBuffer);
    submitInfo.pCommandBuffers		= &vecCmdDraw[currentColorImage];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores	= &drawingCompleteSemaphore;

    // Queue the command buffer for execution
    CommandBufferMgr::submitCommandBuffer(deviceObj->queue, &vecCmdDraw[currentColorImage], &submitInfo);

    // Present the image in the window
    VkPresentInfoKHR present = {};
    present.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present.pNext				= NULL;
    present.swapchainCount		= 1;
    present.pSwapchains			= &swapChain;
    present.pImageIndices		= &currentColorImage;
    present.pWaitSemaphores		= &drawingCompleteSemaphore;
    present.waitSemaphoreCount	= 1;
    present.pResults			= NULL;

    result = swapChainObj->fpQueuePresentKHR(deviceObj->queue, &present);
}
void VulkanDrawable::CreateUniformBuffer() {
    VkResult  result;
    bool  pass;
    Projection	= glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    View		= glm::lookAt(
            glm::vec3(10, 3, 10),	// Camera in World Space
            glm::vec3(0, 0, 0),		// and looks at the origin
            glm::vec3(0, -1, 0)		// Head is up
    );
    Model		= glm::mat4(1.0f);
    MVP			= Projection * View * Model;

    VkBufferCreateInfo bufInfo = {};
    bufInfo.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.pNext					= NULL;
    bufInfo.usage					= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufInfo.size					= sizeof(MVP);
    bufInfo.queueFamilyIndexCount	= 0;
    bufInfo.pQueueFamilyIndices	= NULL;
    bufInfo.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
    bufInfo.flags					= 0;

    result = vkCreateBuffer(deviceObj->device, &bufInfo, NULL, &UniformData.buffer);
    assert(result == VK_SUCCESS);

    VkMemoryRequirements memRequirement;
    vkGetBufferMemoryRequirements(deviceObj->device, UniformData.buffer, &memRequirement);

    VkMemoryAllocateInfo memAllocInfo = {};
    memAllocInfo.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.pNext				= NULL;
    memAllocInfo.memoryTypeIndex		= 0;
    memAllocInfo.allocationSize = memRequirement.size;

    pass = deviceObj->MemoryTypeFromProperties(memRequirement.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &memAllocInfo.memoryTypeIndex);
    assert(pass);

    result = vkAllocateMemory(deviceObj->device, &memAllocInfo, NULL, &(UniformData.memory));
    assert(result == VK_SUCCESS);

    result = vkMapMemory(deviceObj->device, UniformData.memory, 0, memRequirement.size, 0, (void **)&UniformData.pData);

    memcpy(UniformData.pData, &MVP, sizeof(MVP));

    UniformData.mappedRange.resize(1);

    UniformData.mappedRange[0].sType	= VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    UniformData.mappedRange[0].memory	= UniformData.memory;
    UniformData.mappedRange[0].offset	= 0;
    UniformData.mappedRange[0].size		= sizeof(MVP);

    vkInvalidateMappedMemoryRanges(deviceObj->device, 1, &UniformData.mappedRange[0]);

    result = vkBindBufferMemory(deviceObj->device,	UniformData.buffer, UniformData.memory, 0);
    assert(result == VK_SUCCESS);

    UniformData.bufferInfo.buffer	= UniformData.buffer;
    UniformData.bufferInfo.offset	= 0;
    UniformData.bufferInfo.range	= sizeof(MVP);
    UniformData.memRqrmnt			= memRequirement;
}
void VulkanDrawable::CreateDescriptorPool(bool useTexture) {
    VkResult  result;

    std::vector<VkDescriptorPoolSize> descriptorTypePool;

    descriptorTypePool.push_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 });

    if (useTexture) {
        descriptorTypePool.push_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    }

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
    descriptorPoolCreateInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.pNext			= NULL;
    descriptorPoolCreateInfo.maxSets		= 1;
    descriptorPoolCreateInfo.flags			= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    descriptorPoolCreateInfo.poolSizeCount	= (uint32_t)descriptorTypePool.size();
    descriptorPoolCreateInfo.pPoolSizes		= descriptorTypePool.data();

    result = vkCreateDescriptorPool(deviceObj->device,	&descriptorPoolCreateInfo, NULL, &descriptorPool);
    assert(result == VK_SUCCESS);
}
void VulkanDrawable::CreateDescriptorResources() {
    CreateUniformBuffer();
}
void VulkanDrawable::CreateDescriptorSet(bool useTexture) {
    VulkanPipeline* pipelineObj = rendererObj->GetPipelineObject();
    VkResult  result;

    VkDescriptorSetAllocateInfo dsAllocInfo[1];
    dsAllocInfo[0].sType				= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    dsAllocInfo[0].pNext				= NULL;
    dsAllocInfo[0].descriptorPool		= descriptorPool;
    dsAllocInfo[0].descriptorSetCount	= 1;
    dsAllocInfo[0].pSetLayouts			= descLayout.data();

    descriptorSet.resize(1);

    result = vkAllocateDescriptorSets(deviceObj->device, dsAllocInfo, descriptorSet.data());
    assert(result == VK_SUCCESS);

    VkWriteDescriptorSet writes[2];
    memset(&writes, 0, sizeof(writes));

    writes[0]					= {};
    writes[0].sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].pNext				= NULL;
    writes[0].dstSet			= descriptorSet[0];
    writes[0].descriptorCount	= 1;
    writes[0].descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writes[0].pBufferInfo		= &UniformData.bufferInfo;
    writes[0].dstArrayElement	= 0;
    writes[0].dstBinding		= 0; // DESCRIPTOR_SET_BINDING_INDEX

    if (useTexture)
    {
//        VkDescriptorImageInfo desImageInfo{};
//        desImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//        desImageInfo.imageView = textures->view;
//        desImageInfo.sampler = textures->sampler;

        writes[1]					= {};
        writes[1].sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[1].dstSet			= descriptorSet[0];
        writes[1].dstBinding		= 1;
        writes[1].descriptorCount	= 1;
        writes[1].descriptorType	= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writes[1].pImageInfo		= &textures->descsImgInfo;
        writes[1].dstArrayElement	= 0;
    }

    // Update the uniform buffer into the allocated descriptor set
    vkUpdateDescriptorSets(deviceObj->device, useTexture ? 2 : 1, writes, 0, nullptr);
}
void VulkanDrawable::CreateDescriptorSetLayout(bool useTexture) {
    VkDescriptorSetLayoutBinding layoutBindings[2];
    layoutBindings[0].binding				= 0; // DESCRIPTOR_SET_BINDING_INDEX
    layoutBindings[0].descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBindings[0].descriptorCount		= 1;
    layoutBindings[0].stageFlags			= VK_SHADER_STAGE_VERTEX_BIT;
    layoutBindings[0].pImmutableSamplers	= NULL;

    if (useTexture)
    {
        layoutBindings[1].binding				= 1;
        layoutBindings[1].descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        layoutBindings[1].descriptorCount		= 1;
        layoutBindings[1].stageFlags			= VK_SHADER_STAGE_FRAGMENT_BIT;
        layoutBindings[1].pImmutableSamplers	= NULL;
    }

    VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
    descriptorLayout.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorLayout.pNext			= NULL;
    descriptorLayout.bindingCount	= useTexture ? 2 : 1;
    descriptorLayout.pBindings		= layoutBindings;

    VkResult  result;

    descLayout.resize(1);
    result = vkCreateDescriptorSetLayout(deviceObj->device, &descriptorLayout, NULL, descLayout.data());
    assert(result == VK_SUCCESS);
}
void VulkanDrawable::CreatePipelineLayout() {
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
    pPipelineLayoutCreateInfo.sType						= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pPipelineLayoutCreateInfo.pNext						= NULL;
    pPipelineLayoutCreateInfo.pushConstantRangeCount	= 0;
    pPipelineLayoutCreateInfo.pPushConstantRanges		= NULL;
    pPipelineLayoutCreateInfo.setLayoutCount			= (uint32_t)descLayout.size();
    pPipelineLayoutCreateInfo.pSetLayouts				= descLayout.data();

    VkResult  result;
    result = vkCreatePipelineLayout(deviceObj->device, &pPipelineLayoutCreateInfo, NULL, &pipelineLayout);
}
void VulkanDrawable::InitViewports(VkCommandBuffer *cmd) {
    viewport.height		= (float)rendererObj->height;
    viewport.width		= (float)rendererObj->width;
    viewport.minDepth	= (float) 0.0f;
    viewport.maxDepth	= (float) 1.0f;
    viewport.x			= 0;
    viewport.y			= 0;
    vkCmdSetViewport(*cmd, 0, NUMBER_OF_VIEWPORTS, &viewport);
}
void VulkanDrawable::InitScissors(VkCommandBuffer *cmd) {
    scissor.extent.width	= rendererObj->width;
    scissor.extent.height	= rendererObj->height;
    scissor.offset.x		= 0;
    scissor.offset.y		= 0;
    vkCmdSetScissor(*cmd, 0, NUMBER_OF_SCISSORS, &scissor);
}
void VulkanDrawable::DestroyVertexBuffer() {
    vkDestroyBuffer(rendererObj->GetDevice()->device, VertexBuffer.buf, NULL);
    vkFreeMemory(rendererObj->GetDevice()->device, VertexBuffer.mem, NULL);
}
void VulkanDrawable::DestroyCommandBuffer() {
    for (int i = 0; i<vecCmdDraw.size(); i++) {
        vkFreeCommandBuffers(deviceObj->device, rendererObj->cmdPool, 1, &vecCmdDraw[i]);
    }
}
void VulkanDrawable::DestroySynchronizationObjects() {
    vkDestroySemaphore(deviceObj->device, presentCompleteSemaphore, NULL);
    vkDestroySemaphore(deviceObj->device, drawingCompleteSemaphore, NULL);
}
void VulkanDrawable::DestroyUniformBuffer() {
    VkResult  result;
    bool  pass;
    Projection	= glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    View		= glm::lookAt(
            glm::vec3(10, 3, 10),
            glm::vec3(0, 0, 0),
            glm::vec3(0, -1, 0)
    );
    Model		= glm::mat4(1.0f);
    MVP			= Projection * View * Model;

    VkBufferCreateInfo bufInfo = {};
    bufInfo.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.pNext					= NULL;
    bufInfo.usage					= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufInfo.size					= sizeof(MVP);
    bufInfo.queueFamilyIndexCount	= 0;
    bufInfo.pQueueFamilyIndices	= NULL;
    bufInfo.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
    bufInfo.flags					= 0;

    result = vkCreateBuffer(deviceObj->device, &bufInfo, NULL, &UniformData.buffer);
    assert(result == VK_SUCCESS);

    VkMemoryRequirements memRqrmnt;
    vkGetBufferMemoryRequirements(deviceObj->device, UniformData.buffer, &memRqrmnt);

    VkMemoryAllocateInfo memAllocInfo = {};
    memAllocInfo.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.pNext				= NULL;
    memAllocInfo.memoryTypeIndex		= 0;
    memAllocInfo.allocationSize = memRqrmnt.size;

    pass = deviceObj->MemoryTypeFromProperties(memRqrmnt.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &memAllocInfo.memoryTypeIndex);
    assert(pass);

    // Allocate the memory for buffer objects
    result = vkAllocateMemory(deviceObj->device, &memAllocInfo, NULL, &(UniformData.memory));
    assert(result == VK_SUCCESS);

    result = vkMapMemory(deviceObj->device, UniformData.memory, 0, memRqrmnt.size, 0, (void **)&UniformData.pData);
    assert(result == VK_SUCCESS);

    memcpy(UniformData.pData, &MVP, sizeof(MVP));

    UniformData.mappedRange.resize(1);

    UniformData.mappedRange[0].sType	= VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    UniformData.mappedRange[0].memory	= UniformData.memory;
    UniformData.mappedRange[0].offset	= 0;
    UniformData.mappedRange[0].size		= sizeof(MVP);

    vkInvalidateMappedMemoryRanges(deviceObj->device, 1, &UniformData.mappedRange[0]);

    // Bind the buffer device memory
    result = vkBindBufferMemory(deviceObj->device,	UniformData.buffer, UniformData.memory, 0);
    assert(result == VK_SUCCESS);

    // Update the local data structure with uniform buffer for house keeping
    UniformData.bufferInfo.buffer	= UniformData.buffer;
    UniformData.bufferInfo.offset	= 0;
    UniformData.bufferInfo.range	= sizeof(MVP);
    UniformData.memRqrmnt			= memRqrmnt;
}
void VulkanDrawable::SetTextures(TextureData *tex) {
    textures = tex;
}

//Private
void VulkanDrawable::recordCommandBuffer(int currentImage, VkCommandBuffer *cmdDraw) {
    VulkanDevice* deviceObj			= rendererObj->GetDevice();
    VulkanPipeline* pipelineObj 	= rendererObj->GetPipelineObject();

    // Specify the clear color value
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0] = {0.0f, 0.0f, 0.0f, 1.0f};
    clearValues[1] = {1.0f, 0};

    // Define the VkRenderPassBeginInfo control structure
    VkRenderPassBeginInfo renderPassBegin;
    renderPassBegin.sType						= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBegin.pNext						= NULL;
    renderPassBegin.renderPass					= rendererObj->renderPass;
    renderPassBegin.framebuffer					= rendererObj->framebuffers[currentImage];
    renderPassBegin.renderArea.offset.x			= 0;
    renderPassBegin.renderArea.offset.y			= 0;
    renderPassBegin.renderArea.extent.width		= rendererObj->width;
    renderPassBegin.renderArea.extent.height	= rendererObj->height;
    renderPassBegin.clearValueCount				= static_cast<uint32_t>(clearValues.size());
    renderPassBegin.pClearValues				= clearValues.data();

     VkResult error;

    // Start recording the render pass instance
    vkCmdBeginRenderPass(*cmdDraw, &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE);

    // Bound the command buffer with the graphics pipeline
    vkCmdBindPipeline(*cmdDraw, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);

    vkCmdBindDescriptorSets(*cmdDraw, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
                            0, 1, descriptorSet.data(), 0, nullptr);
    const VkDeviceSize offsets[1] = { 0 };
    vkCmdBindVertexBuffers(*cmdDraw, 0, 1, &VertexBuffer.buf, offsets);

    InitViewports(cmdDraw);
    InitScissors(cmdDraw);

    vkCmdDraw(*cmdDraw, 3 * 2 * 6, 1, 0, 0);

    vkCmdEndRenderPass(*cmdDraw);
}
