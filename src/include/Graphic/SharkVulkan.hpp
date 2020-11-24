//
// Created by HADMARINE on 2020/11/24.
//

#ifndef SHARKENGINE_SHARKVULKAN_HPP
#define SHARKENGINE_SHARKVULKAN_HPP

#include "IncludeVulkan.hpp"
#include "SharkVulkanDefineStructs.hpp"
#include "stdafx.hpp"

#define SHARK_API_CORE SharkEngine::Core::SharkVulkan::getCore()
#define SHARK_API_DEVICE SHARK_API_CORE->GetDevice()

namespace SharkEngine::Core {
    class SharkVulkan {
    public:
        /// STATIC GETTERS
        static SharkVulkan *getCore();


        /// MEMBER GETTERS
        VkDevice *getDevice();
        std::vector<VkCommandBuffer> getCommandBuffers();

        /// CORE FUNCTIONS

        void run();
        void initWindow();
        void initVulkan();

        /// UTIL FUNCTIONS

        void *LoadTextures(std::string location);


        /// GRAPHICS FUNCTIONS


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
        VkDescriptorSetLayout deescriptorSetLayout;

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

        // DEBUG
        VkDebugUtilsMessengerEXT debugMessenger;
    };

}// namespace SharkEngine::Core

#endif//SHARKENGINE_SHARKVULKAN_HPP
