//
// Created by EunwooSong on 2020-10-27.
//

#include "../../include/Graphic/VulkanRenderer.h"

#include "../../GlobalPreferences.hpp"
#include "../../include/Graphic/VulkanApplication.h"
#include "../../include/Graphic/VulkanDevice.h"
#include "../../include/Graphic/VulkanMeshStruct.h"

/*********** IMAGE HEADER FILES ***********/
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

VulkanRenderer::VulkanRenderer(VulkanApplication *app, VulkanDevice *deviceObject) {
    memset(&Depth, 0, sizeof(Depth));

    application = app;
    deviceObj = deviceObject;

    swapChainObj = new VulkanSwapChain(this);
    VulkanDrawable *drawableObj = new VulkanDrawable(this);
    drawableList.push_back(drawableObj);
}
VulkanRenderer::~VulkanRenderer() {
    delete swapChainObj;
    swapChainObj = NULL;
    for (auto d : drawableList) {
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
    CreateTextureLinear("../src/source/texture2.jpg", VK_IMAGE_USAGE_SAMPLED_BIT);
    for (VulkanDrawable *drawableObj : drawableList) {
        drawableObj->SetTextures(tmp);
    }
    CreateDescriptors();
    CreatePipelineStateManagement();
}
void VulkanRenderer::Prepare() {
    for (VulkanDrawable *drawableObj : drawableList) {
        drawableObj->Prepare();
    }
}
void VulkanRenderer::Update() {
    for (VulkanDrawable *drawableObj : drawableList) {
        drawableObj->Update();
    }
}
bool VulkanRenderer::Render() {
    if (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        for (VulkanDrawable *drawableObj : drawableList) {
            drawableObj->Render();
        }
        return true;
    }

    vkDeviceWaitIdle(application->deviceObj->device);
    return false;
}

void VulkanRenderer::CreatePresentationWindow() {
    width = GlobalPreferences::SCREEN_WIDTH;
    height = GlobalPreferences::SCREEN_HEIGHT;

    window = glfwCreateWindow(
            GlobalPreferences::SCREEN_WIDTH,
            GlobalPreferences::SCREEN_HEIGHT,
            GlobalPreferences::APPLICATION_NAME,
            nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
}

void VulkanRenderer::FramebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto app = reinterpret_cast<VulkanRenderer *>(glfwGetWindowUserPointer(window));

    app->width = width;
    app->height = height;
    app->GetSwapChain()->SetSwapChainExtent(width, height);
    app->application->Resize();
}

void VulkanRenderer::DestroyPresentationWindow() {
    glfwDestroyWindow(window);
}

void VulkanRenderer::SetImageLayout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, const VkImageSubresourceRange &subresourceRange, VkCommandBuffer const &cmdBuf) {
    assert(cmdBuf != VK_NULL_HANDLE);

    assert(deviceObj->queue != VK_NULL_HANDLE);

    VkImageMemoryBarrier imgMemoryBarrier = {};
    imgMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imgMemoryBarrier.pNext = NULL;
    imgMemoryBarrier.srcAccessMask = 0;
    imgMemoryBarrier.dstAccessMask = 0;
    imgMemoryBarrier.oldLayout = oldImageLayout;
    imgMemoryBarrier.newLayout = newImageLayout;
    imgMemoryBarrier.image = image;
    imgMemoryBarrier.subresourceRange = subresourceRange;

    if (oldImageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        imgMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }

    // Source layouts (old)
    switch (oldImageLayout) {
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

    switch (newImageLayout) {
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

    VkPipelineStageFlags srcStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags destStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    vkCmdPipelineBarrier(cmdBuf, srcStages, destStages, 0, 0, NULL, 0, NULL, 1, &imgMemoryBarrier);
}
void VulkanRenderer::CreateCommandPool() {
    VulkanDevice *deviceObj = application->deviceObj;
    /* Depends on intializeSwapChainExtension() */
    VkResult res;

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
    VkResult result;
    bool pass;

    VkImageCreateInfo imageInfo = {};

    // If the depth format is undefined, use fallback as 16-byte value
    if (Depth.format == VK_FORMAT_UNDEFINED) {
        Depth.format = VK_FORMAT_D16_UNORM;
    }

    const VkFormat depthFormat = Depth.format;

    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext = NULL;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = depthFormat;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = NUM_SAMPLES;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.queueFamilyIndexCount = 0;
    imageInfo.pQueueFamilyIndices = NULL;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.flags = 0;

    // User create image info and create the image objects
    result = vkCreateImage(deviceObj->device, &imageInfo, NULL, &Depth.image);
    assert(result == VK_SUCCESS);

    // Get the image memory requirements
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(deviceObj->device, Depth.image, &memRequirements);

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
    imgViewInfo.components = {VK_COMPONENT_SWIZZLE_IDENTITY};
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

    for (VulkanDrawable *drawableObj : drawableList) {
        drawableObj->CreateVertexBuffer(geometryData, sizeof(geometryData), sizeof(geometryData[0]), true);
    }

    CommandBufferMgr::endCommandBuffer(cmdVertexBuffer);
    CommandBufferMgr::submitCommandBuffer(deviceObj->queue, &cmdVertexBuffer);
}
void VulkanRenderer::CreateRenderPass(bool includeDepth, bool clear) {
    VkResult result;
    // Attach the color buffer and depth buffer as an attachment to render pass instance
    VkAttachmentDescription attachments[2];
    attachments[0].format = swapChainObj->scPublicVars.format;
    attachments[0].samples = NUM_SAMPLES;
    attachments[0].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachments[0].flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;

    // Is the depth buffer present the define attachment properties for depth buffer attachment.
    if (includeDepth) {
        attachments[1].format = Depth.format;
        attachments[1].samples = NUM_SAMPLES;
        attachments[1].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachments[1].flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
    }

    // Define the color buffer attachment binding point and layout information
    VkAttachmentReference colorReference = {};
    colorReference.attachment = 0;
    colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Define the depth buffer attachment binding point and layout information
    VkAttachmentReference depthReference = {};
    depthReference.attachment = 1;
    depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Specify the attachments - color, depth, resolve, preserve etc.
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags = 0;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = NULL;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorReference;
    subpass.pResolveAttachments = NULL;
    subpass.pDepthStencilAttachment = includeDepth ? &depthReference : NULL;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = NULL;

    // Specify the attachement and subpass associate with render pass
    VkRenderPassCreateInfo rpInfo = {};
    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpInfo.pNext = NULL;
    rpInfo.attachmentCount = includeDepth ? 2 : 1;
    rpInfo.pAttachments = attachments;
    rpInfo.subpassCount = 1;
    rpInfo.pSubpasses = &subpass;
    rpInfo.dependencyCount = 0;
    rpInfo.pDependencies = NULL;

    // Create the render pass object
    result = vkCreateRenderPass(deviceObj->device, &rpInfo, NULL, &renderPass);

    assert(result == VK_SUCCESS);
}
void VulkanRenderer::CreateFrameBuffer(bool includeDepth) {
    VkResult result;
    VkImageView attachments[2];
    attachments[1] = Depth.view;

    VkFramebufferCreateInfo fbInfo = {};
    fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbInfo.pNext = NULL;
    fbInfo.renderPass = renderPass;
    fbInfo.attachmentCount = includeDepth ? 2 : 1;
    fbInfo.pAttachments = attachments;
    fbInfo.width = width;
    fbInfo.height = height;
    fbInfo.layers = 1;

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

    void *vertShaderCode, *fragShaderCode;
    size_t sizeVert, sizeFrag;

#ifdef AUTO_COMPILE_GLSL_TO_SPV
    vertShaderCode = readFile("./../Texture.vert", &sizeVert);
    fragShaderCode = readFile("./../Texture.frag", &sizeFrag);

    shaderObj.buildShader((const char *) vertShaderCode, (const char *) fragShaderCode);
#else
    //../src/source/texture2.jpg
    vertShaderCode = readFile("../src/shaders/vert.spv", &sizeVert);
    fragShaderCode = readFile("../src/shaders/frag.spv", &sizeFrag);

    shaderObj.BuildShaderModuleWithSPV((uint32_t *) vertShaderCode, sizeVert, (uint32_t *) fragShaderCode, sizeFrag);
#endif
}
void VulkanRenderer::CreatePipelineStateManagement() {
    for (VulkanDrawable *drawableObj : drawableList) {
        // Use the descriptor layout and create the pipeline layout.
        drawableObj->CreatePipelineLayout();
    }

    pipelineObj.CreatePipelineCache();

    const bool depthPresent = true;
    for (VulkanDrawable *drawableObj : drawableList) {
        VkPipeline *pipeline = (VkPipeline *) malloc(sizeof(VkPipeline));
        if (pipelineObj.CreatePipeline(drawableObj, pipeline, &shaderObj, depthPresent)) {
            pipelineList.push_back(pipeline);
            drawableObj->SetPipeline(pipeline);
        } else {
            free(pipeline);
            pipeline = NULL;
        }
    }
}
void VulkanRenderer::CreateDescriptors() {
    for (VulkanDrawable *drawableObj : drawableList) {
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
void VulkanRenderer::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    CommandBufferMgr::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdTexture);
    CommandBufferMgr::beginCommandBuffer(cmdTexture);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        CLogger::Error("Unsupported layout transition");
        throw std::invalid_argument("Unsupported layout transition");
    }

    vkCmdPipelineBarrier(
            cmdTexture,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdTexture;

    vkQueueSubmit(deviceObj->queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(deviceObj->queue);

    CommandBufferMgr::endCommandBuffer(cmdTexture);
}
void VulkanRenderer::CreateTextureLinear(std::string filename, VkImageUsageFlags imageUsageFlags, VkFormat format) {
    int texWidth, texHeight, texChannels;

    stbi_uc *pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    TextureData* texture = new TextureData;
    texture->textureWidth = texWidth;
    texture->textureHeight = texHeight;
    texture->mipMapLevels = 1;
    if (!pixels) {
        CLogger::Error("Failed to load texture image");
        throw std::runtime_error("Failed to load texture image");
    }

    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size	= imageSize;
    bufferCreateInfo.usage	= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult error;
    // Create a buffer resource (host-visible) -
    VkBuffer buffer;
    error = vkCreateBuffer(deviceObj->device, &bufferCreateInfo, NULL, &buffer);
    assert(!error);

    // Get the buffer memory requirements for the staging buffer -
    VkMemoryRequirements memRqrmnt;
    VkDeviceMemory	devMemory;
    vkGetBufferMemoryRequirements(deviceObj->device, buffer, &memRqrmnt);

    VkMemoryAllocateInfo memAllocInfo = {};
    memAllocInfo.sType			= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.pNext			= NULL;
    memAllocInfo.allocationSize = 0;
    memAllocInfo.memoryTypeIndex= 0;
    memAllocInfo.allocationSize = memRqrmnt.size;

    // Determine the type of memory required for the host-visible buffer  -
    deviceObj->MemoryTypeFromProperties(memRqrmnt.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &memAllocInfo.memoryTypeIndex);

    // Allocate the memory for host-visible buffer objects -
    error = vkAllocateMemory(deviceObj->device, &memAllocInfo, nullptr, &devMemory);
    assert(!error);

    // Bind the host-visible buffer with allocated device memory -
    error = vkBindBufferMemory(deviceObj->device, buffer, devMemory, 0);
    assert(!error);

    // Populate the raw image data into the device memory -
    uint32_t *data;
    error = vkMapMemory(deviceObj->device, devMemory, 0, memRqrmnt.size, 0, (void **)&data);
    assert(!error);

    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(deviceObj->device, devMemory);

    // Create image info with optimal tiling support (.tiling = VK_IMAGE_TILING_OPTIMAL) -
    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType			= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext			= NULL;
    imageCreateInfo.imageType		= VK_IMAGE_TYPE_2D;
    imageCreateInfo.format			= format;
    imageCreateInfo.mipLevels		= texture->mipMapLevels;
    imageCreateInfo.arrayLayers		= 1;
    imageCreateInfo.samples			= VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling			= VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.sharingMode		= VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.extent			= { texture->textureWidth, texture->textureHeight, 1 };
    imageCreateInfo.usage			= imageUsageFlags;

    // Set image object with VK_IMAGE_USAGE_TRANSFER_DST_BIT if
    // not set already. This allows to copy the source VkBuffer
    // object (with VK_IMAGE_USAGE_TRANSFER_DST_BIT) contents
    // into this image object memory(destination).
    if (!(imageCreateInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT)){
        imageCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }

    error = vkCreateImage(deviceObj->device, &imageCreateInfo, nullptr, &texture->image);
    assert(!error);

    // Get the image memory requirements
    vkGetImageMemoryRequirements(deviceObj->device, texture->image, &memRqrmnt);

    // Set the allocation size equal to the buffer allocation
    memAllocInfo.allocationSize = memRqrmnt.size;

    // Determine the type of memory required with the help of memory properties
    deviceObj->MemoryTypeFromProperties(memRqrmnt.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memAllocInfo.memoryTypeIndex);

    // Allocate the physical memory on the GPU
    error = vkAllocateMemory(deviceObj->device, &memAllocInfo, nullptr, &texture->mem);
    assert(!error);

    // Bound the physical memory with the created image object
    error = vkBindImageMemory(deviceObj->device, texture->image, texture->mem, 0);
    assert(!error);

    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask				= VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel			= 0;
    subresourceRange.levelCount				= texture->mipMapLevels;
    subresourceRange.layerCount				= 1;

    // Use a separate command buffer for texture loading
    // Start command buffer recording
    CommandBufferMgr::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdTexture);
    CommandBufferMgr::beginCommandBuffer(cmdTexture);

    // set the image layout to be
    // VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    // since it is destination for copying buffer
    // into image using vkCmdCopyBufferToImage -
    SetImageLayout(texture->image, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED,
                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange, cmdTexture);

    // List contains the buffer image copy for each mipLevel -
    std::vector<VkBufferImageCopy> bufferImgCopyList;


    uint32_t bufferOffset = 0;
    // Iterater through each mip level and set buffer image copy -
    for (uint32_t i = 0; i < texture->mipMapLevels; i++)
    {
        VkBufferImageCopy bufImgCopyItem = {};
        bufImgCopyItem.imageSubresource.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
        bufImgCopyItem.imageSubresource.mipLevel		= i;
        bufImgCopyItem.imageSubresource.layerCount		= 1;
        bufImgCopyItem.imageSubresource.baseArrayLayer	= 0;
        bufImgCopyItem.imageExtent.width				= uint32_t(texWidth);
        bufImgCopyItem.imageExtent.height				= uint32_t(texHeight);
        bufImgCopyItem.imageExtent.depth				= 1;
        bufImgCopyItem.bufferOffset						= bufferOffset;

        bufferImgCopyList.push_back(bufImgCopyItem);

        // adjust buffer offset
        bufferOffset += uint32_t(imageSize);
    }

    // Copy the staging buffer memory data contain
    // the stage raw data(with mip levels) into image object
    vkCmdCopyBufferToImage(cmdTexture, buffer, texture->image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           uint32_t(bufferImgCopyList.size()), bufferImgCopyList.data());

    // Advised to change the image layout to shader read
    // after staged buffer copied into image memory -
    texture->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SetImageLayout(texture->image, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                   texture->imageLayout, subresourceRange, cmdTexture);


    // Submit command buffer containing copy and image layout commands-
    CommandBufferMgr::endCommandBuffer(cmdTexture);

    // Create a fence object to ensure that the command buffer is executed,
    // coping our staged raw data from the buffers to image memory with
    // respective image layout and attributes into consideration -
    VkFence fence;
    VkFenceCreateInfo fenceCI	= {};
    fenceCI.sType				= VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCI.flags				= 0;

    error = vkCreateFence(deviceObj->device, &fenceCI, nullptr, &fence);
    assert(!error);

    VkSubmitInfo submitInfo			= {};
    submitInfo.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext				= NULL;
    submitInfo.commandBufferCount	= 1;
    submitInfo.pCommandBuffers		= &cmdTexture;

    CommandBufferMgr::submitCommandBuffer(deviceObj->queue, &cmdTexture, &submitInfo, fence);

    error = vkWaitForFences(deviceObj->device, 1, &fence, VK_TRUE, 10000000000);
    assert(!error);

    vkDestroyFence(deviceObj->device, fence, nullptr);

    // destroy the allocated resoureces
    vkFreeMemory(deviceObj->device, devMemory, nullptr);
    vkDestroyBuffer(deviceObj->device, buffer, nullptr);

    ///////////////////////////////////////////////////////////////////////////////////////

    // Create sampler
    VkSamplerCreateInfo samplerCI = {};
    samplerCI.sType						= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCI.pNext						= NULL;
    samplerCI.magFilter					= VK_FILTER_LINEAR;
    samplerCI.minFilter					= VK_FILTER_LINEAR;
    samplerCI.mipmapMode				= VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCI.addressModeU				= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.addressModeV				= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.addressModeW				= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.mipLodBias				= 0.0f;
    if (deviceObj->deviceFeatures.samplerAnisotropy == VK_TRUE)
    {
        samplerCI.anisotropyEnable		= VK_TRUE;
        samplerCI.maxAnisotropy			= 8;
    }
    else
    {
        samplerCI.anisotropyEnable		= VK_FALSE;
        samplerCI.maxAnisotropy			= 1;
    }
    samplerCI.compareOp					= VK_COMPARE_OP_NEVER;
    samplerCI.minLod					= 0.0f;
    samplerCI.maxLod					= (float)texture->mipMapLevels;
    samplerCI.borderColor				= VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    samplerCI.unnormalizedCoordinates	= VK_FALSE;

    error = vkCreateSampler(deviceObj->device, &samplerCI, nullptr, &texture->sampler);
    assert(!error);

    // Create image view to allow shader to access the texture information -
    VkImageViewCreateInfo viewCI = {};
    viewCI.sType			= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewCI.pNext			= NULL;
    viewCI.viewType			= VK_IMAGE_VIEW_TYPE_2D;
    viewCI.format			= format;
    viewCI.components.r		= VK_COMPONENT_SWIZZLE_R;
    viewCI.components.g		= VK_COMPONENT_SWIZZLE_G;
    viewCI.components.b		= VK_COMPONENT_SWIZZLE_B;
    viewCI.components.a		= VK_COMPONENT_SWIZZLE_A;
    viewCI.subresourceRange	= subresourceRange;
    viewCI.subresourceRange.levelCount	= texture->mipMapLevels; 	// Optimal tiling supports mip map levels very well set it.
    viewCI.image			= texture->image;

    error = vkCreateImageView(deviceObj->device, &viewCI, NULL, &texture->view);
    assert(!error);

    // Fill descriptor image info that can be used for setting up descriptor sets
    texture->descsImgInfo.imageView = texture->view;
    texture->descsImgInfo.sampler = texture->sampler;
    texture->descsImgInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

    tmp = texture;
}

void VulkanRenderer::CreateTextureOptimal(std::string filename, VkImageUsageFlags imageUsageFlags, VkFormat format) {
    CreateTextureLinear(filename, imageUsageFlags, format);
}
void VulkanRenderer::DestroyCommandBuffer() {
    VkCommandBuffer cmdBuffers[] = {cmdDepthImage, cmdVertexBuffer, cmdTexture};
    vkFreeCommandBuffers(deviceObj->device, cmdPool, sizeof(cmdBuffers) / sizeof(VkCommandBuffer), cmdBuffers);
}
void VulkanRenderer::DestroyCommandPool() {
    VulkanDevice *deviceObj = application->deviceObj;

    vkDestroyCommandPool(deviceObj->device, cmdPool, NULL);
}
void VulkanRenderer::DestroyDepthBuffer() {
    vkDestroyImageView(deviceObj->device, Depth.view, NULL);
    vkDestroyImage(deviceObj->device, Depth.image, NULL);
    vkFreeMemory(deviceObj->device, Depth.mem, NULL);
}
void VulkanRenderer::DestroyDrawableVertexBuffer() {
    for (VulkanDrawable *drawableObj : drawableList) {
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
    for (VkPipeline *pipeline : pipelineList) {
        vkDestroyPipeline(deviceObj->device, *pipeline, NULL);
        free(pipeline);
    }
    pipelineList.clear();
}
void VulkanRenderer::DestroyDrawableCommandBuffer() {
    for (VulkanDrawable *drawableObj : drawableList) {
        drawableObj->DestroyCommandBuffer();
    }
}
void VulkanRenderer::DestroyDrawableSynchronizationObjects() {
    for (VulkanDrawable *drawableObj : drawableList) {
        drawableObj->DestroySynchronizationObjects();
    }
}
void VulkanRenderer::DestroyDrawableUniformBuffer() {
    for (VulkanDrawable *drawableObj : drawableList) {
        drawableObj->DestroyUniformBuffer();
    }
}
void VulkanRenderer::DestroyTextureResource() {
    for (auto iter : texturesData) {
        TextureData texture = *iter.second;
        vkFreeMemory(deviceObj->device, texture.mem, NULL);
        vkDestroySampler(deviceObj->device, texture.sampler, NULL);
        vkDestroyImage(deviceObj->device, texture.image, NULL);
        vkDestroyImageView(deviceObj->device, texture.view, NULL);
    }
}
VkImageView VulkanRenderer::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
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
    if (vkCreateImageView(deviceObj->device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        CLogger::Error("Failed to create texture image view");
        throw std::runtime_error("Failed to create texture image view");
    }

    return imageView;
}
void VulkanRenderer::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (!(imageInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT)){
        imageInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }

    if (vkCreateImage(deviceObj->device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        CLogger::Error("Failed to create image");
        throw std::runtime_error("Failed to create image");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(deviceObj->device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = deviceObj->FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(deviceObj->device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        CLogger::Error("Failed to allocate image memory");
        throw std::runtime_error("Failed to allocate image memory");
    }

    vkBindImageMemory(deviceObj->device, image, imageMemory, 0);
}
void VulkanRenderer::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer;
    CommandBufferMgr::allocCommandBuffer(&deviceObj->device, cmdPool, &commandBuffer);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
            width,
            height,
            1};

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    EndSingleTimeCommands(commandBuffer);
}

VkCommandBuffer VulkanRenderer::BeginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = cmdPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(deviceObj->device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void VulkanRenderer::EndSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(deviceObj->queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(deviceObj->queue);

    vkFreeCommandBuffers(deviceObj->device, cmdPool, 1, &commandBuffer);

}

