//
// Created by EunwooSong on 2020-10-27.
//

#include "../../include/Graphic/VulkanRenderer.h"

#include "../../include/Graphic/VulkanApplication.h"
#include "../../include/Graphic/VulkanDevice.h"
#include "../../include/Graphic/VulkanMeshStruct.h"
#include "../../GlobalPreferences.hpp"

/*********** IMAGE HEADER FILES ***********/
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

VulkanRenderer::VulkanRenderer(VulkanApplication *app, VulkanDevice *deviceObject) {
    memset(&Depth, 0, sizeof(Depth));
    application = app;
    deviceObj	= deviceObject;

    swapChainObj = new VulkanSwapChain(this);
    //VulkanDrawable* drawableObj = new VulkanDrawable(this);
    //drawableList.push_back(drawableObj);
}
VulkanRenderer::~VulkanRenderer() {
    delete swapChainObj;
    swapChainObj = NULL;
    for(auto d : drawableList)
    {
        delete d;
    }
    drawableList.clear();
}
void VulkanRenderer::Initialize() {
    CreateCommandPool();

    BuildSwapChainAndDepthImage();

    CreateVertexBuffer();

    const bool includeDepth = true;
    // Create the render pass now . . .
    CreateRenderPass(includeDepth);

    // Use render pass and create frame buffer
    CreateFrameBuffer(includeDepth);

    CreateShaders();

    //CreateTextureLinear("../../source/texture.jpg", VK_IMAGE_USAGE_SAMPLED_BIT);
    //CreateTextureLinear("../src/source/texture2.jpg", VK_IMAGE_USAGE_SAMPLED_BIT);

    CreateDescriptors();
    CreatePipelineStateManagement();
}
void VulkanRenderer::Prepare() {
    for(VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->Prepare();
    }
}
void VulkanRenderer::Update() {
    for(VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->Update();
    }
}
bool VulkanRenderer::Render() {
    if(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        for (VulkanDrawable* drawableObj : drawableList)
        {
            drawableObj->Render();
        }
        return true;
    }

    vkDeviceWaitIdle(application->deviceObj->device);
    return false;
}

void VulkanRenderer::CreatePresentationWindow() {
    width	= GlobalPreferences::SCREEN_WIDTH;
    height	= GlobalPreferences::SCREEN_HEIGHT;

    window = glfwCreateWindow(
            GlobalPreferences::SCREEN_WIDTH,
            GlobalPreferences::SCREEN_HEIGHT,
            GlobalPreferences::APPLICATION_NAME,
            nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
}

void VulkanRenderer::FramebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto app = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));

    app->width = width;
    app->height = height;
    app->GetSwapChain()->SetSwapChainExtent(width, height);
    app->application->Resize();
}

void VulkanRenderer::DestroyPresentationWindow() {
    glfwDestroyWindow(window);
}

void VulkanRenderer::SetImageLayout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, const VkImageSubresourceRange &subresourceRange, VkCommandBuffer const &cmdBuf) {
    VkImageMemoryBarrier imgMemoryBarrier = {};
    imgMemoryBarrier.sType			= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imgMemoryBarrier.pNext			= NULL;
    imgMemoryBarrier.srcAccessMask	= 0;
    imgMemoryBarrier.dstAccessMask	= 0;
    imgMemoryBarrier.oldLayout		= oldImageLayout;
    imgMemoryBarrier.newLayout		= newImageLayout;
    imgMemoryBarrier.image			= image;
    imgMemoryBarrier.subresourceRange = subresourceRange;

    if (oldImageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        imgMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }

    // Source layouts (old)
    switch (oldImageLayout)
    {
        case VK_IMAGE_LAYOUT_UNDEFINED:
            imgMemoryBarrier.srcAccessMask = 0;
            break;
        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            imgMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imgMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imgMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }

    switch (newImageLayout)
    {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
            imgMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            imgMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
            imgMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imgMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            imgMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
            break;
    }

    VkPipelineStageFlags srcStages	= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags destStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    vkCmdPipelineBarrier(cmdBuf, srcStages, destStages, 0, 0, NULL, 0, NULL, 1, &imgMemoryBarrier);

}
void VulkanRenderer::CreateCommandPool() {
    VulkanDevice* deviceObj		= application->deviceObj;
    /* Depends on intializeSwapChainExtension() */
    VkResult  res;

    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.pNext = NULL;
    cmdPoolInfo.queueFamilyIndex = deviceObj->graphicsQueueWithPresentIndex;
    cmdPoolInfo.flags = 0;

    res = vkCreateCommandPool(deviceObj->device, &cmdPoolInfo, NULL, &cmdPool);
}
void VulkanRenderer::BuildSwapChainAndDepthImage() {
    deviceObj->GetDeviceQueue();

    swapChainObj->CreateSwapChain(cmdDepthImage);

    CreateDepthImage();
}
void VulkanRenderer::CreateDepthImage() {
    VkResult  result;
    bool  pass;

    VkImageCreateInfo imageInfo = {};

    // If the depth format is undefined, use fallback as 16-byte value
    if (Depth.format == VK_FORMAT_UNDEFINED) {
        Depth.format = VK_FORMAT_D16_UNORM;
    }

    const VkFormat depthFormat = Depth.format;

    imageInfo.sType			= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext			= NULL;
    imageInfo.imageType		= VK_IMAGE_TYPE_2D;
    imageInfo.format		= depthFormat;
    imageInfo.extent.width	= width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth	= 1;
    imageInfo.mipLevels		= 1;
    imageInfo.arrayLayers	= 1;
    imageInfo.samples		= NUM_SAMPLES;
    imageInfo.tiling		= VK_IMAGE_TILING_OPTIMAL;
    imageInfo.queueFamilyIndexCount = 0;
    imageInfo.pQueueFamilyIndices	= NULL;
    imageInfo.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.usage					= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.flags					= 0;

    // User create image info and create the image objects
    result = vkCreateImage(deviceObj->device, &imageInfo, NULL, &Depth.image);
    assert(result == VK_SUCCESS);

    // Get the image memory requirements
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(deviceObj->device,	Depth.image, &memRequirements);

    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloc.pNext = NULL;
    memAlloc.allocationSize = 0;
    memAlloc.memoryTypeIndex = 0;
    memAlloc.allocationSize = memRequirements.size;
    // Determine the type of memory required with the help of memory properties
    pass = deviceObj->MemoryTypeFromProperties(memRequirements.memoryTypeBits, 0, &memAlloc.memoryTypeIndex);

    // Allocate the memory for image objects
    result = vkAllocateMemory(deviceObj->device, &memAlloc, NULL, &Depth.mem);

    // Bind the allocated memeory
    result = vkBindImageMemory(deviceObj->device, Depth.image, Depth.mem, 0);


    VkImageViewCreateInfo imgViewInfo = {};
    imgViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imgViewInfo.pNext = NULL;
    imgViewInfo.image = VK_NULL_HANDLE;
    imgViewInfo.format = depthFormat;
    imgViewInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY };
    imgViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    imgViewInfo.subresourceRange.baseMipLevel = 0;
    imgViewInfo.subresourceRange.levelCount = 1;
    imgViewInfo.subresourceRange.baseArrayLayer = 0;
    imgViewInfo.subresourceRange.layerCount = 1;
    imgViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imgViewInfo.flags = 0;

    if (depthFormat == VK_FORMAT_D16_UNORM_S8_UINT ||
        depthFormat == VK_FORMAT_D24_UNORM_S8_UINT ||
        depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT) {
        imgViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    // Use command buffer to create the depth image. This includes -
    // Command buffer allocation, recording with begin/end scope and submission.
    CommandBufferMgr::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdDepthImage);
    CommandBufferMgr::beginCommandBuffer(cmdDepthImage);
    {
        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = 1;
        subresourceRange.layerCount = 1;

        SetImageLayout(Depth.image,
                       imgViewInfo.subresourceRange.aspectMask,
                       VK_IMAGE_LAYOUT_UNDEFINED,
                       VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, subresourceRange, cmdDepthImage);
    }
    CommandBufferMgr::endCommandBuffer(cmdDepthImage);
    CommandBufferMgr::submitCommandBuffer(deviceObj->queue, &cmdDepthImage);

    imgViewInfo.image = Depth.image;
    result = vkCreateImageView(deviceObj->device, &imgViewInfo, NULL, &Depth.view);
    assert(result == VK_SUCCESS);
}
void VulkanRenderer::CreateVertexBuffer() {
    CommandBufferMgr::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdVertexBuffer);
    CommandBufferMgr::beginCommandBuffer(cmdVertexBuffer);

    for (VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->CreateVertexBuffer(geometryData, sizeof(geometryData), sizeof(geometryData[0]), false);
    }
    CommandBufferMgr::endCommandBuffer(cmdVertexBuffer);
    CommandBufferMgr::submitCommandBuffer(deviceObj->queue, &cmdVertexBuffer);

}
void VulkanRenderer::CreateRenderPass(bool includeDepth, bool clear) {
    VkResult  result;
    // Attach the color buffer and depth buffer as an attachment to render pass instance
    VkAttachmentDescription attachments[2];
    attachments[0].format					= swapChainObj->scPublicVars.format;
    attachments[0].samples					= NUM_SAMPLES;
    attachments[0].loadOp					= clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].storeOp					= VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp			= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp			= VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout			= VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout				= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachments[0].flags					= VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;

    // Is the depth buffer present the define attachment properties for depth buffer attachment.
    if (includeDepth)
    {
        attachments[1].format				= Depth.format;
        attachments[1].samples				= NUM_SAMPLES;
        attachments[1].loadOp				= clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[1].storeOp				= VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].stencilLoadOp		= VK_ATTACHMENT_LOAD_OP_LOAD;
        attachments[1].stencilStoreOp		= VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].initialLayout		= VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[1].finalLayout			= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachments[1].flags				= VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
    }

    // Define the color buffer attachment binding point and layout information
    VkAttachmentReference colorReference	= {};
    colorReference.attachment				= 0;
    colorReference.layout					= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Define the depth buffer attachment binding point and layout information
    VkAttachmentReference depthReference = {};
    depthReference.attachment				= 1;
    depthReference.layout					= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Specify the attachments - color, depth, resolve, preserve etc.
    VkSubpassDescription subpass			= {};
    subpass.pipelineBindPoint				= VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags							= 0;
    subpass.inputAttachmentCount			= 0;
    subpass.pInputAttachments				= NULL;
    subpass.colorAttachmentCount			= 1;
    subpass.pColorAttachments				= &colorReference;
    subpass.pResolveAttachments				= NULL;
    subpass.pDepthStencilAttachment			= includeDepth ? &depthReference : NULL;
    subpass.preserveAttachmentCount			= 0;
    subpass.pPreserveAttachments			= NULL;

    // Specify the attachement and subpass associate with render pass
    VkRenderPassCreateInfo rpInfo			= {};
    rpInfo.sType							= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpInfo.pNext							= NULL;
    rpInfo.attachmentCount					= includeDepth ? 2 : 1;
    rpInfo.pAttachments						= attachments;
    rpInfo.subpassCount						= 1;
    rpInfo.pSubpasses						= &subpass;
    rpInfo.dependencyCount					= 0;
    rpInfo.pDependencies					= NULL;

    // Create the render pass object
    result = vkCreateRenderPass(deviceObj->device, &rpInfo, NULL, &renderPass);

}
void VulkanRenderer::CreateFrameBuffer(bool includeDepth) {
    VkResult  result;
    VkImageView attachments[2];
    attachments[1] = Depth.view;

    VkFramebufferCreateInfo fbInfo	= {};
    fbInfo.sType					= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbInfo.pNext					= NULL;
    fbInfo.renderPass				= renderPass;
    fbInfo.attachmentCount			= includeDepth ? 2 : 1;
    fbInfo.pAttachments				= attachments;
    fbInfo.width					= width;
    fbInfo.height					= height;
    fbInfo.layers					= 1;

    uint32_t i;

    framebuffers.clear();
    framebuffers.resize(swapChainObj->scPublicVars.swapchainImageCount);
    for (i = 0; i < swapChainObj->scPublicVars.swapchainImageCount; i++) {
        attachments[0] = swapChainObj->scPublicVars.colorBuffer[i].view;
        result = vkCreateFramebuffer(deviceObj->device, &fbInfo, NULL, &framebuffers.at(i));
        assert(result == VK_SUCCESS);
    }
}
void VulkanRenderer::CreateShaders() {
    if (application->isResizing)
        return;

    void* vertShaderCode, *fragShaderCode;
    size_t sizeVert, sizeFrag;

#ifdef AUTO_COMPILE_GLSL_TO_SPV
    vertShaderCode = readFile("./../Texture.vert", &sizeVert);
	fragShaderCode = readFile("./../Texture.frag", &sizeFrag);

	shaderObj.buildShader((const char*)vertShaderCode, (const char*)fragShaderCode);
#else
    vertShaderCode = readFile("../src/shaders/vert.spv", &sizeVert);
    fragShaderCode = readFile("../src/shaders/frag.spv", &sizeFrag);

    shaderObj.BuildShaderModuleWithSPV((uint32_t*)vertShaderCode, sizeVert, (uint32_t*)fragShaderCode, sizeFrag);
#endif
}
void VulkanRenderer::CreatePipelineStateManagement() {
    for (VulkanDrawable* drawableObj : drawableList)
    {
        // Use the descriptor layout and create the pipeline layout.
        drawableObj->CreatePipelineLayout();
    }

    pipelineObj.CreatePipelineCache();

    const bool depthPresent = true;
    for (VulkanDrawable* drawableObj : drawableList)
    {
        VkPipeline* pipeline = (VkPipeline*)malloc(sizeof(VkPipeline));
        if (pipelineObj.CreatePipeline(drawableObj, pipeline, &shaderObj, depthPresent))
        {
            pipelineList.push_back(pipeline);
            drawableObj->SetPipeline(pipeline);
        }
        else
        {
            free(pipeline);
            pipeline = NULL;
        }
    }
}
void VulkanRenderer::CreateDescriptors() {
    for (VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->CreateDescriptorSetLayout(true);

        // Create the descriptor set
        drawableObj->CreateDescriptor(true);
    }
}
void VulkanRenderer::CreateStagingBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(deviceObj->device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        CLogger::Error("Failed to create vertex buffer");
        throw std::runtime_error("Failed to create vertex buffer");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(deviceObj->device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = deviceObj->FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(deviceObj->device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        CLogger::Error("Failed to allocate vertex buffer memory");
        throw std::runtime_error("Failed to allocate vertex buffer memory");
    }

    vkBindBufferMemory(deviceObj->device, buffer, bufferMemory, 0);
}
void VulkanRenderer::CreateTextureLinear(const char *filename, VkImageUsageFlags imageUsageFlags, VkFormat format) {
    if(texturesData[filename])
        return;

    int texWidth, texHeight, texChannels;
    stbi_uc *pixels = stbi_load(filename, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        CLogger::Error("Failed to load texture image %s", filename);
        throw std::runtime_error("Failed to load texture image");
    }

    TextureData* texture = new TextureData();

    texture->textureWidth = static_cast<uint32_t>(texWidth);
    texture->textureWidth = static_cast<uint32_t>(texHeight);
    texture->mipMapLevels = static_cast<uint32_t>(texChannels);

    // Create image resource states using VkImageCreateInfo
    VkImageCreateInfo imageInfo   = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = texWidth;
    imageInfo.extent.height = texHeight;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult  error;
    // Use create image info and create the image objects
    error = vkCreateImage(deviceObj->device, &imageInfo, NULL, &texture->image);

    // Get the buffer memory requirements
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(deviceObj->device, texture->image, &memoryRequirements);

    // Create memory allocation metadata information
    VkMemoryAllocateInfo& memAlloc	= texture->memoryAlloc;
    memAlloc.sType					= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloc.pNext					= NULL;
    memAlloc.allocationSize			= memoryRequirements.size;
    memAlloc.memoryTypeIndex		= 0;

    // Determine the type of memory required
    // with the help of memory properties
    bool pass = deviceObj->MemoryTypeFromProperties(memoryRequirements.memoryTypeBits,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &texture->memoryAlloc.memoryTypeIndex);

    // Allocate the memory for buffer objects
    error = vkAllocateMemory(deviceObj->device, &texture->memoryAlloc, NULL, &(texture->mem));

    // Bind the image device memory
    error = vkBindImageMemory(deviceObj->device, texture->image, texture->mem, 0);

    VkImageSubresource subresource	= {};
    subresource.aspectMask			= VK_IMAGE_ASPECT_COLOR_BIT;
    subresource.mipLevel			= 0;
    subresource.arrayLayer			= 0;

    //Create staging buffer for get data
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateStagingBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        stagingBuffer, stagingBufferMemory);
    VkSubresourceLayout layout;
    void* data;
    vkMapMemory(deviceObj->device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, stagingBufferMemory);
    vkUnmapMemory(deviceObj->device, stagingBufferMemory);

    stbi_image_free(pixels);

    vkGetImageSubresourceLayout(deviceObj->device, texture->image, &subresource, &layout);

    // Map the GPU memory on to local host
    error = vkMapMemory(deviceObj->device, texture->mem, 0, texture->memoryAlloc.allocationSize, 0, (void**)&data);
    assert(!error);

    // UnMap the host memory to push the changes into the device memory
    vkUnmapMemory(deviceObj->device, texture->mem);

    // Command buffer allocation and recording begins
    CommandBufferMgr::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdTexture);
    CommandBufferMgr::beginCommandBuffer(cmdTexture);

    VkImageSubresourceRange subresourceRange	= {};
    subresourceRange.aspectMask					= VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel				= 0;
    subresourceRange.levelCount					= texture->mipMapLevels;
    subresourceRange.layerCount					= 1;

    texture->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SetImageLayout(texture->image, VK_IMAGE_ASPECT_COLOR_BIT,
                   VK_IMAGE_LAYOUT_PREINITIALIZED, texture->imageLayout,
                   subresourceRange, cmdTexture);

    // Stop command buffer recording
    CommandBufferMgr::endCommandBuffer(cmdTexture);

    // Ensure that the GPU has finished the submitted job before host takes over again
    VkFence fence;
    VkFenceCreateInfo fenceCI = {};
    fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCI.flags = 0;

    vkCreateFence(deviceObj->device, &fenceCI, nullptr, &fence);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdTexture;

    CommandBufferMgr::submitCommandBuffer(deviceObj->queue, &cmdTexture, &submitInfo, fence);
    vkWaitForFences(deviceObj->device, 1, &fence, VK_TRUE, 10000000000);
    vkDestroyFence(deviceObj->device, fence, nullptr);

    // Specify a particular kind of texture using samplers
    VkSamplerCreateInfo samplerCI	= {};
    samplerCI.sType					= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCI.pNext					= NULL;
    samplerCI.magFilter				= VK_FILTER_LINEAR;
    samplerCI.minFilter				= VK_FILTER_LINEAR;
    samplerCI.mipmapMode			= VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCI.addressModeU			= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.addressModeV			= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.addressModeW			= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.mipLodBias			= 0.0f;
    if (deviceObj->deviceFeatures.samplerAnisotropy == VK_TRUE)
    {
        samplerCI.anisotropyEnable	= VK_TRUE;
        samplerCI.maxAnisotropy		= 8;
    }
    else
    {
        samplerCI.anisotropyEnable	= VK_FALSE;
        samplerCI.maxAnisotropy		= 1;
    }
    samplerCI.compareOp				= VK_COMPARE_OP_NEVER;
    samplerCI.minLod				= 0.0f;
    samplerCI.maxLod				= 0.0f; // Set to texture->mipLevels if Optimal tiling, generally linear does not support mip-maping
    samplerCI.borderColor			= VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    samplerCI.unnormalizedCoordinates = VK_FALSE;

    // Create the sampler
    error = vkCreateSampler(deviceObj->device, &samplerCI, NULL, &texture->sampler);
    assert(!error);

    // Create image view to allow shader to access the texture information -
    VkImageViewCreateInfo viewCI	= {};
    viewCI.sType					= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewCI.pNext					= NULL;
    viewCI.viewType					= VK_IMAGE_VIEW_TYPE_2D;
    viewCI.format					= format;
    viewCI.components.r				= VK_COMPONENT_SWIZZLE_R;
    viewCI.components.g				= VK_COMPONENT_SWIZZLE_G;
    viewCI.components.b				= VK_COMPONENT_SWIZZLE_B;
    viewCI.components.a				= VK_COMPONENT_SWIZZLE_A;
    viewCI.subresourceRange			= subresourceRange;
    viewCI.subresourceRange.levelCount = 1;
    viewCI.flags					= 0;
    viewCI.image					= texture->image;

    error = vkCreateImageView(deviceObj->device, &viewCI, NULL, &texture->view);
    assert(!error);

    texture->descsImgInfo.sampler		= texture->sampler;
    texture->descsImgInfo.imageView		= texture->view;
    texture->descsImgInfo.imageLayout	= VK_IMAGE_LAYOUT_GENERAL;

    //Save texture data
    texturesData[filename] = texture;
}
void VulkanRenderer::CreateTextureOptimal(const char *filename, VkImageUsageFlags imageUsageFlags, VkFormat format) {
    CreateTextureLinear(filename, imageUsageFlags, format);
}
void VulkanRenderer::DestroyCommandBuffer() {
    VkCommandBuffer cmdBuffers[] = { cmdDepthImage, cmdVertexBuffer, cmdTexture };
    vkFreeCommandBuffers(deviceObj->device, cmdPool, sizeof(cmdBuffers) / sizeof(VkCommandBuffer), cmdBuffers);
}
void VulkanRenderer::DestroyCommandPool() {
    VulkanDevice* deviceObj = application->deviceObj;

    vkDestroyCommandPool(deviceObj->device, cmdPool, NULL);
}
void VulkanRenderer::DestroyDepthBuffer() {
    vkDestroyImageView(deviceObj->device, Depth.view, NULL);
    vkDestroyImage(deviceObj->device, Depth.image, NULL);
    vkFreeMemory(deviceObj->device, Depth.mem, NULL);
}
void VulkanRenderer::DestroyDrawableVertexBuffer() {
    for (VulkanDrawable* drawableObj : drawableList) {
        drawableObj->DestroyVertexBuffer();
    }
}
void VulkanRenderer::DestroyRenderPass() {
    vkDestroyRenderPass(deviceObj->device, renderPass, NULL);
}
void VulkanRenderer::DestroyFrameBuffers() {
    for (uint32_t i = 0; i < swapChainObj->scPublicVars.swapchainImageCount; i++) {
        vkDestroyFramebuffer(deviceObj->device, framebuffers.at(i), NULL);
    }
    framebuffers.clear();
}
void VulkanRenderer::DestroyPipeline() {
    for (VkPipeline* pipeline : pipelineList)
    {
        vkDestroyPipeline(deviceObj->device, *pipeline, NULL);
        free(pipeline);
    }
    pipelineList.clear();
}
void VulkanRenderer::DestroyDrawableCommandBuffer() {
    for (VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->DestroyCommandBuffer();
    }
}
void VulkanRenderer::DestroyDrawableSynchronizationObjects() {
    for (VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->DestroySynchronizationObjects();
    }
}
void VulkanRenderer::DestroyDrawableUniformBuffer() {
    for (VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->DestroyUniformBuffer();
    }
}
void VulkanRenderer::DestroyTextureResource() {
    for(auto iter : texturesData) {
        TextureData texture = *iter.second;
        vkFreeMemory(deviceObj->device, texture.mem, NULL);
        vkDestroySampler(deviceObj->device, texture.sampler, NULL);
        vkDestroyImage(deviceObj->device, texture.image, NULL);
        vkDestroyImageView(deviceObj->device, texture.view, NULL);
    }
}
