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
    void CreatePresentationWindow(const int& windowWidth = 500, const int& windowHeight = 500);
    void SetImageLayout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, const VkImageSubresourceRange& subresourceRange, const VkCommandBuffer& cmdBuf);

    //! Windows procedure method for handling events.
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
    void CreateTextureLinear (const char* filename, VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM);
    void CreateTextureOptimal(const char* filename, VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM);

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
#ifdef _WIN32
#define APP_NAME_STR_LEN 80
    HINSTANCE					connection;				// hInstance - Windows Instance
    char						name[APP_NAME_STR_LEN]; // name - App name appearing on the window
    HWND						window;					// hWnd - the window handle
#else
    xcb_connection_t*			connection;
	xcb_screen_t*				screen;
	xcb_window_t				window;
	xcb_intern_atom_reply_t*	reply;
#endif

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
    std::vector<VkFramebuffer> framebuffers;	// Number of frame buffer corresponding to each swap chain
    std::vector<VkPipeline*> pipelineList;		// List of pipelines

    //임시 텍스쳐 메니저
    int					width, height;
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
