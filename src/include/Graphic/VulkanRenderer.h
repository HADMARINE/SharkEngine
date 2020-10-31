//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_VULKANRENDERER_H
#define SHARKENGINE_VULKANRENDERER_H

#include "IncludeVulkan.h"
#include "VulkanSwapChain.h"
#include "VulkanDrawable.h"
#include "VulkanShader.h"
#include "VulkanPipeline.h"
#include "../../stdafx.hpp"

#include <GLFW/glfw3.h>

#define NUM_SAMPLES VK_SAMPLE_COUNT_1_BIT

class VulkanRenderer {
public:
    VulkanRenderer(VulkanApplication* app, VulkanDevice* deviceObject);
    ~VulkanRenderer();

public:
    void Initialize();
    void Prepare();
    void Update();
    bool Render();

    // Create an empty window
    void CreatePresentationWindow();

    static void FramebufferResizeCallback(GLFWwindow *window, int width, int height);

    void SetImageLayout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, const VkImageSubresourceRange& subresourceRange, const VkCommandBuffer& cmdBuf);

    // Destroy the presentation window
    void DestroyPresentationWindow();

    // Getter functions for member variable specific to classes.
    inline VulkanApplication*               GetApplication()		{ return application; }
    inline VulkanDevice*                    GetDevice()				{ return deviceObj; }
    inline VulkanSwapChain*                 GetSwapChain() 		    { return swapChainObj; }
    inline std::vector<VulkanDrawable*>*    GetDrawingItems()       { return &drawableList; }
    inline VkCommandPool*                   GetCommandPool()		{ return &cmdPool; }
    inline VulkanShader*                    GetShader()				{ return &shaderObj; }
    inline VulkanPipeline*	                GetPipelineObject()		{ return &pipelineObj; }

    void CreateCommandPool();							// Create command pool
    void BuildSwapChainAndDepthImage();					// Create swapchain color image and depth image
    void CreateDepthImage();							// Create depth image
    void CreateVertexBuffer();
    void CreateRenderPass(bool includeDepth, bool clear = true);	// Render Pass creation
    void CreateFrameBuffer(bool includeDepth);
    void CreateShaders();
    void CreatePipelineStateManagement();
    void CreateDescriptors();

    void CreateStagingBuffer(VkDeviceSize size,
                             VkBufferUsageFlags usage,
                             VkMemoryPropertyFlags properties,
                             VkBuffer &buffer,
                             VkDeviceMemory &bufferMemory);
    void CreateTextureLinear (const char* filename, VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT, VkFormat format = VK_FORMAT_R8G8B8A8_SRGB);
    void CreateTextureOptimal(const char* filename, VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT, VkFormat format = VK_FORMAT_R8G8B8A8_SRGB);

    void DestroyCommandBuffer();
    void DestroyCommandPool();
    void DestroyDepthBuffer();
    void DestroyDrawableVertexBuffer();
    void DestroyRenderPass();										// Destroy the render pass object when no more required
    void DestroyFrameBuffers();
    void DestroyPipeline();
    void DestroyDrawableCommandBuffer();
    void DestroyDrawableSynchronizationObjects();
    void DestroyDrawableUniformBuffer();
    void DestroyTextureResource();
public:
    GLFWwindow *window;

    struct{
        VkFormat		format;
        VkImage			image;
        VkDeviceMemory	mem;
        VkImageView		view;
    }Depth;

    VkCommandBuffer		cmdDepthImage;			// Command buffer for depth image layout
    VkCommandPool		cmdPool;				// Command pool
    VkCommandBuffer		cmdVertexBuffer;		// Command buffer for vertex buffer - Triangle geometry
    VkCommandBuffer		cmdTexture;				// Command buffer for creating the texture

    VkRenderPass		renderPass;				// Render pass created object
    std::vector<VkFramebuffer>  framebuffers;	// Number of frame buffer corresponding to each swap chain
    std::vector<VkPipeline*>    pipelineList;		// List of pipelines

    int					width, height;

    //임시 텍스쳐 메니저
    std::map<const char*, TextureData*> texturesData;

private:
    VulkanApplication* application;

    VulkanDevice*	   deviceObj;
    VulkanSwapChain*   swapChainObj;
    std::vector<VulkanDrawable*> drawableList;
    VulkanShader 	   shaderObj;
    VulkanPipeline 	   pipelineObj;
};


#endif//SHARKENGINE_VULKANRENDERER_H
