//
// Created by HADMARINE on 2020/11/24.
//

#ifndef SHARKENGINE_SHARKVULKAN_HPP
#define SHARKENGINE_SHARKVULKAN_HPP

#include "stdafx.hpp"

#include "IncludeVulkan.hpp"
#include "SharkVulkanDefine.hpp"
#include "SharkVulkanDrawable.hpp"

namespace SharkEngine::Core {
    class SharkVulkan {
    public:
        /// STATIC GETTERS
        static SharkVulkan *GetCore();


        /// MEMBER GETTERS
        VkDevice *GetDevice();
        std::vector<VkCommandBuffer> GetCommandBuffers();
        std::vector<VkBuffer> GetUniformBuffers();
        std::vector<VkImage> GetSwapChainImages();
        VkDescriptorPool GetDescriptorPool();
        VkDescriptorSetLayout GetDescriptorSetLayout();
        VkSampler GetTextureSampler();
        std::vector<SharkVulkanDrawable>* GetDrawables();
        uint32_t AppendDrawable(const SharkVulkanDrawable& drawable);
        void DeleteDrawable(uint32_t pos);

        /// CORE FUNCTIONS

        void Run();
        void MainLoop();
        void Cleanup();

        void InitWindow();
        void InitVulkan();

        /// GRAPHICS FUNCTIONS

        void DrawFrame();

        void CreateInstance();
        void CreateSurface();
        void CreateLogicalDevice();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateDescriptorSetLayout();
        void CreateGraphicsPipeline();
        void CreateCommandPool();
        void CreateDepthResources();
        void CreateFrameBuffers();
        void CreateTextureSampler();
        void CreateVertexBuffer();
        void CreateIndexBuffer();
        void CreateUniformBuffers();
        void CreateDescriptorPool();
        void CreateDescriptorSets();
        void CreateCommandBuffer();
        void CreateSyncObjects();

//        void UpdateVertexBuffer();
        void UpdateUniformBuffer(uint32_t currentImage);

        // CLEANUP
        void CleanupSwapChain();
        void RecreateSwapChain();



        /// UTIL FUNCTIONS

        static void FramebufferResizeCallback(GLFWwindow *window, int width, int height);

        static std::vector<char> ReadFile(const std::string &filename);
        void *LoadTextures(std::string location);

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                     VkFormatFeatureFlags features);
        VkFormat FindDepthFormat();
        bool HasStencilComponent(VkFormat format);
        void PickPhysicalDevice();
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        bool IsDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        std::vector<const char*> getRequiredExtensions();
        bool CheckValidationLayerSupport();

        void CreateImage(uint32_t width, uint32_t height, VkFormat format,
                         VkImageTiling tiling, VkImageUsageFlags usage,
                         VkMemoryPropertyFlags properties, VkImage &image,
                         VkDeviceMemory &imageMemory);
        VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void TransitionImageLayout(VkImage image, VkFormat format,
                                    VkImageLayout oldLayout, VkImageLayout newLayout);
        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                          VkMemoryPropertyFlags properties, VkBuffer &buffer,
                          VkDeviceMemory &bufferMemory);
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        VkShaderModule CreateShaderModule(const std::vector<char> &code);

        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);


        /// DEBUG LAYERS

        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        void SetupDebugMessenger();

    private:
        /// MEMBER VARIABLES

        std::vector<uint32_t> indices;
        std::vector<Vertex> vertices;

        GLFWwindow *window;

        VkInstance instance;

        VkDevice device;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;


        VkQueue graphicsQueue;
        VkQueue presentQueue;


        // SWAPCHAIN
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        // RENDER PASS
        VkRenderPass renderPass;

        // DESCRIPTOR
        VkDescriptorPool descriptorPool;
        VkDescriptorSetLayout descriptorSetLayout;
        std::vector<std::vector<VkDescriptorSet>> descriptorSets;

        // PIPELINE
        VkPipeline graphicsPipeline;
        VkPipelineLayout pipelineLayout;

        // COMMAND BUFFER
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        // IMAGE CONCURRENT RENDER
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame = 0;

        // TEXTURE SAMPLER
        VkSampler textureSampler;

        // DEPTH IMAGE
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        // VERTEX / INDEX / UNIFORM BUFFERS
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;

        // DRAWABLE
        std::vector<SharkVulkanDrawable> drawables;

        // DEBUG
        VkDebugUtilsMessengerEXT debugMessenger;

        // ETC
        bool framebufferResized = false;
    };

}// namespace SharkEngine::Core

#endif//SHARKENGINE_SHARKVULKAN_HPP
