//
// Created by EunwooSong on 2020-11-02.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define STB_IMAGE_IMPLEMENTATION

#include "../../stdafx.hpp"
#include <GLFW/glfw3.h>
#include <Vulkan/vulkan.h>
#include <array>
#include <cstring>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>

namespace VulkanCore {
#define VULKAN_CORE VulkanEngine::Instance()

    const std::vector<const char *> validationLayers = {
            "VK_LAYER_KHRONOS_validation"};

    const std::vector<const char *> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct Vertex {
        glm::vec3 pos;
        glm::vec4 color;
        glm::vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

            return attributeDescriptions;
        }
    };

    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    class TextureImageStruct {
    public:
        TextureImageStruct() {}
        ~TextureImageStruct() {}

        VkImage image;
        VkDeviceMemory deviceMemory;
    };

    struct TextureData {
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
    };

    //class GraphicsObject {
    //private:
    //    std::vector<Vertex> vertices;
    //    std::vector<uint16_t> indices;
    //    TextureImageStruct *texture;
    //
    //public:
    //    static int indexCount;
    //    std::vector<Vertex> getVertices() { return vertices; };
    //    std::vector<uint16_t> getIndices() { return indices; };
    //    TextureImageStruct *getTexture() const { return texture; };
    //    GraphicsObject(const std::vector<Vertex> &vertices, TextureImageStruct *texture) {
    //        this->vertices = vertices;
    //        this->texture = texture;
    //        for (int i = 0; i < vertices.size() - 2; i++) {
    //            this->indices.push_back(indexCount);
    //            this->indices.push_back(indexCount + i + 1);
    //            this->indices.push_back(indexCount + i + 2);
    //        }
    //        indexCount += vertices.size();
    //    }
    //};

    class VulkanEngine {
    public:
        static VulkanEngine *Instance() {
            static VulkanEngine *instance = new VulkanEngine();
            return instance;
        }

        void run(const std::vector<TextureImageStruct> *texImgStructs) {
            //        initWindow();
            //        initVulkan();
            //        mainLoop();
            //        cleanup(texImgStructs);
        }

        void Initialize() {
            initWindow();
            initVulkan();
        }

        bool Render() {
            if (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
                drawFrame();
                return true;
            }

            vkDeviceWaitIdle(device);
            return false;
        }

        void Release() {
            cleanup();
        }

        TextureImageStruct createTextureImage(const std::string location);

        GLFWwindow *GetWindow() { return window; }
        VkDevice &GetDevice() { return device; }
        VkInstance &GetInstance() { return instance; }
        VkCommandPool &GetCommandPool() { return commandPool; }
        VkRenderPass &GetRenderPass() { return renderPass; }
        std::vector<VkFramebuffer> &GetSwapChainFrameBuffer() { return swapChainFramebuffers; }
        VkExtent2D GetSwapChainExtent() { return swapChainExtent; }
        VkPipeline GetGraphicPipeline() { return graphicsPipeline; }
        int GetCurrentFrame() { return currentFrame; }
        VkSwapchainKHR &GetSwapChain() { return swapChain; }
        VkQueue &GetGraphicQueue() { return graphicsQueue; }
        VkQueue &GetPresentQueue() { return presentQueue; }
        VkFence &GetFrameInFlightFence(int index) { return (0 <= index && index < inFlightFences.size()) ? inFlightFences[index] : inFlightFences[0]; }
        VkFence &GetCurrentFrameInFlightFences() { return inFlightFences[currentFrame]; }
        VkDescriptorSetLayout &GetDescriptorSetLayout() { return descriptorSetLayout; }
        VkPipelineLayout &GetPipelineLayout() { return pipelineLayout; }

        //Util
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                          VkBuffer &buffer, VkDeviceMemory &bufferMemory) {
            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usage;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
                CLogger::Error("Failed to create vertex buffer");
                throw std::runtime_error("Failed to create vertex buffer");
            }

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
                CLogger::Error("Failed to allocate vertex buffer memory");
                throw std::runtime_error("Failed to allocate vertex buffer memory");
            }

            vkBindBufferMemory(device, buffer, bufferMemory, 0);
        }

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = commandPool;
            allocInfo.commandBufferCount = 1;

            VkCommandBuffer commandBuffer;
            vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(commandBuffer, &beginInfo);

            VkBufferCopy copyRegion{};
            copyRegion.srcOffset = 0;
            copyRegion.dstOffset = 0;
            copyRegion.size = size;
            vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

            vkEndCommandBuffer(commandBuffer);

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(graphicsQueue);

            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
        }

        int getSwapChainImageCount() { return swapChainImages.size(); }

        std::vector<Vertex> vertices = {
                {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
                {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},

                {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
                {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
                {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
                {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

        std::vector<uint16_t> indices = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4};

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
            if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
                CLogger::Error("Failed to create texture image view");
                throw std::runtime_error("Failed to create texture image view");
            }

            return imageView;
        }

    private:
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                              const VkAllocationCallbacks *pAllocator,
                                              VkDebugUtilsMessengerEXT *pDebugMessenger) {
            auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
            if (func != nullptr) {
                return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
            } else {
                return VK_ERROR_EXTENSION_NOT_PRESENT;
            }
        }

        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks *pAllocator) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                                    "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                func(instance, debugMessenger, pAllocator);
            }
        }


        //    Member variable
        GLFWwindow *window;

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        VkRenderPass renderPass;
        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;


        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame = 0;

        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;

        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

        bool framebufferResized = false;

        void initWindow() {
            glfwInit();

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            window = glfwCreateWindow(
                    GlobalPreferences::SCREEN_WIDTH,
                    GlobalPreferences::SCREEN_HEIGHT,
                    GlobalPreferences::APPLICATION_NAME,
                    nullptr, nullptr);
            glfwSetWindowUserPointer(window, this);
            glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        }

        static void framebufferResizeCallback(GLFWwindow *window, int width, int height) {
            auto app = reinterpret_cast<VulkanEngine *>(glfwGetWindowUserPointer(window));
            app->framebufferResized = true;
        }


        void initVulkan() {
            createInstance();
            setupDebugMessenger();
            createSurface();
            pickPhysicalDevice();
            createLogicalDevice();
            createSwapChain();
            createImageViews();
            createRenderPass();
            createDescriptorSetLayout();
            createGraphicsPipeline();
            createCommandPool();
            createDepthResources();
            createFrameBuffers();

            //auto textureImageInitial = createTextureImage("texture.jpg");

            //
        }


        void cleanup() {//const std::vector<TextureImageStruct> *textureImageStructs) {
            cleanupSwapChain();

            //        Free TextureStruct array

            //        for (TextureImageStruct textureImageStruct : *textureImageStructs) {
            //            vkDestroyImage(device, *textureImageStruct.image, nullptr);
            //            vkFreeMemory(device, *textureImageStruct.deviceMemory, nullptr);
            //        }
            //        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
            //        vkDestroyBuffer(device, indexBuffer, nullptr);
            //        vkFreeMemory(device, indexBufferMemory, nullptr);
            //
            //        vkDestroyBuffer(device, vertexBuffer, nullptr);
            //        vkFreeMemory(device, vertexBufferMemory, nullptr);

            //        for (size_t i = 0; i < GlobalPreferences::MAX_FRAMES_IN_FLIGHT; i++) {
            //            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            //            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            //            vkDestroyFence(device, inFlightFences[i], nullptr);
            //        }

            // Drawable Manager -> cleanup();
            //VULKAN_DRAW_MANAGER->ReleaseAll();

            vkDestroyCommandPool(device, commandPool, nullptr);

            vkDestroyDevice(device, nullptr);

            if (GlobalPreferences::enableValidationLayers) {
                DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
            }

            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyInstance(instance, nullptr);

            glfwDestroyWindow(window);

            glfwTerminate();
        }

        void cleanupSwapChain() {
            vkDestroyImageView(device, depthImageView, nullptr);
            vkDestroyImage(device, depthImage, nullptr);
            vkFreeMemory(device, depthImageMemory, nullptr);

            for (auto &swapChainFramebuffer : swapChainFramebuffers) {
                vkDestroyFramebuffer(device, swapChainFramebuffer, nullptr);
            }

            vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

            vkDestroyPipeline(device, graphicsPipeline, nullptr);
            vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
            vkDestroyRenderPass(device, renderPass, nullptr);

            for (auto &swapChainImageView : swapChainImageViews) {
                vkDestroyImageView(device, swapChainImageView, nullptr);
            }

            vkDestroySwapchainKHR(device, swapChain, nullptr);

            //Drawable Manager -> cleanupSwapChain();
            //        for (size_t i = 0; i < swapChainImages.size(); i++) {
            //            vkDestroyBuffer(device, uniformBuffers[i], nullptr);
            //            vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
            //        }
            //
            //        vkDestroyDescriptorPool(device, descriptorPool, nullptr);
        }

        static std::vector<char> readFile(const std::string &fileName) {
            std::ifstream file(fileName, std::ios::ate | std::ios::binary);

            if (!file.is_open()) {
                CLogger::Error("Failed to open file : %s", fileName.c_str());
                throw std::runtime_error("Failed to open file");
            }

            size_t fileSize = (size_t) file.tellg();
            std::vector<char> buffer(fileSize);

            file.seekg(0);
            file.read(buffer.data(), fileSize);
            file.close();

            return buffer;
        }

        void drawFrame() {

            //VULKAN_DRAW_MANAGER->Render();

            if (framebufferResized) {
                framebufferResized = false;
                recreateSwapChain();
            }//else if (result != VK_SUCCESS) {
             //            CLogger::Error("Failed to present swap chain image");
             //            throw std::runtime_error("Failed to present swap chain image");
             //        }

            currentFrame = (currentFrame + 1) % GlobalPreferences::MAX_FRAMES_IN_FLIGHT;
        }

        void updateUniformBuffer(uint32_t currentImage) {
            static auto startTime = std::chrono::high_resolution_clock::now();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

            UniformBufferObject ubo{};
            ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f),
                                    glm::vec3(0.0f, 0.0f, 1.0f));
            ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 5.0f),
                                   glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            // Field of view
            ubo.proj = glm::perspective(glm::radians(45.0f),
                                        swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 100.0f);

            ubo.proj[1][1] *= -1;

            void *data;
            vkMapMemory(device, uniformBuffersMemory[currentImage], 0,
                        sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
        }

        //    void createSyncObjects() {
        //        imageAvailableSemaphores.resize(GlobalPreferences::MAX_FRAMES_IN_FLIGHT);
        //        renderFinishedSemaphores.resize(GlobalPreferences::MAX_FRAMES_IN_FLIGHT);
        //        inFlightFences.resize(GlobalPreferences::MAX_FRAMES_IN_FLIGHT);
        //        imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);
        //
        //        VkSemaphoreCreateInfo semaphoreInfo{};
        //        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        //
        //        VkFenceCreateInfo fenceInfo{};
        //        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        //        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        //
        //        for (size_t i = 0; i < GlobalPreferences::MAX_FRAMES_IN_FLIGHT; i++) {
        //            if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
        //                vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
        //                vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
        //
        //                CLogger::Error("Failed to create semaphores for a frame");
        //                throw std::runtime_error("Failed to create semaphores for a frame");
        //            }
        //        }
        //    }

        void createGraphicsPipeline() {
            auto vertShaderCode = readFile("../src/shaders/vert.spv");
            auto fragShaderCode = readFile("../src/shaders/frag.spv");

            VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
            VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

            VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
            vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            vertShaderStageInfo.module = vertShaderModule;
            vertShaderStageInfo.pName = "main";

            VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
            fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            fragShaderStageInfo.module = fragShaderModule;
            fragShaderStageInfo.pName = "main";

            VkPipelineShaderStageCreateInfo shaderStages[] =
                    {vertShaderStageInfo, fragShaderStageInfo};

            VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

            auto bindingDescription = Vertex::getBindingDescription();
            auto attributeDescriptions = Vertex::getAttributeDescriptions();

            vertexInputInfo.vertexBindingDescriptionCount = 1;
            vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
            vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
            vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

            VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
            inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            inputAssembly.primitiveRestartEnable = VK_FALSE;

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float) swapChainExtent.width;
            viewport.height = (float) swapChainExtent.height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = swapChainExtent;

            VkPipelineViewportStateCreateInfo viewportState{};
            viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewportState.viewportCount = 1;
            viewportState.pViewports = &viewport;
            viewportState.scissorCount = 1;
            viewportState.pScissors = &scissor;

            VkPipelineRasterizationStateCreateInfo rasterizer{};
            rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterizer.depthClampEnable = VK_FALSE;
            rasterizer.rasterizerDiscardEnable = VK_FALSE;
            rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
            rasterizer.lineWidth = 1.0f;
            rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
            rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
            rasterizer.depthBiasEnable = VK_FALSE;

            VkPipelineMultisampleStateCreateInfo multisampling{};
            multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multisampling.sampleShadingEnable = VK_FALSE;
            multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            multisampling.minSampleShading = 1.0f;
            multisampling.pSampleMask = nullptr;
            multisampling.alphaToCoverageEnable = VK_FALSE;
            multisampling.alphaToOneEnable = VK_FALSE;

            VkPipelineDepthStencilStateCreateInfo depthStencil{};
            depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            depthStencil.depthTestEnable = VK_TRUE;
            depthStencil.depthWriteEnable = VK_TRUE;
            depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
            depthStencil.depthBoundsTestEnable = VK_FALSE;
            depthStencil.minDepthBounds = 0.0f;
            depthStencil.maxDepthBounds = 1.0f;
            depthStencil.stencilTestEnable = VK_FALSE;

            VkPipelineColorBlendAttachmentState colorBlendAttachment{};
            colorBlendAttachment.colorWriteMask =
                    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            colorBlendAttachment.blendEnable = VK_TRUE;
            colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
            colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_SUBTRACT;

            VkPipelineColorBlendStateCreateInfo colorBlending{};
            colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            colorBlending.logicOpEnable = VK_FALSE;
            colorBlending.logicOp = VK_LOGIC_OP_COPY;
            colorBlending.attachmentCount = 1;
            colorBlending.pAttachments = &colorBlendAttachment;
            colorBlending.blendConstants[0] = 0.0f;
            colorBlending.blendConstants[1] = 0.0f;
            colorBlending.blendConstants[2] = 0.0f;
            colorBlending.blendConstants[3] = 0.0f;

            VkDynamicState dynamicStates[] = {
                    VK_DYNAMIC_STATE_VIEWPORT,
                    VK_DYNAMIC_STATE_LINE_WIDTH};

            VkPipelineDynamicStateCreateInfo dynamicState{};
            dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            dynamicState.dynamicStateCount = 2;
            dynamicState.pDynamicStates = dynamicStates;

            VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = 1;
            pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
            pipelineLayoutInfo.pPushConstantRanges = 0;
            pipelineLayoutInfo.pPushConstantRanges = nullptr;

            if (vkCreatePipelineLayout(device, &pipelineLayoutInfo,
                                       nullptr, &pipelineLayout) != VK_SUCCESS) {
                CLogger::Error("Failed to create pipeline layout");
                throw std::runtime_error("Failed to create pipeline layout");
            }

            VkGraphicsPipelineCreateInfo pipelineInfo{};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.stageCount = 2;
            pipelineInfo.pStages = shaderStages;
            pipelineInfo.pVertexInputState = &vertexInputInfo;
            pipelineInfo.pInputAssemblyState = &inputAssembly;
            pipelineInfo.pViewportState = &viewportState;
            pipelineInfo.pRasterizationState = &rasterizer;
            pipelineInfo.pMultisampleState = &multisampling;
            pipelineInfo.pDepthStencilState = nullptr;
            pipelineInfo.pColorBlendState = &colorBlending;
            pipelineInfo.pDynamicState = nullptr;
            pipelineInfo.layout = pipelineLayout;
            pipelineInfo.renderPass = renderPass;
            pipelineInfo.subpass = 0;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
            pipelineInfo.basePipelineIndex = -1;
            pipelineInfo.pDepthStencilState = &depthStencil;

            if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE,
                                          1, &pipelineInfo, nullptr,
                                          &graphicsPipeline) != VK_SUCCESS) {
                CLogger::Error("Failed to create graphics pipeline");
                throw std::runtime_error("Failed to create graphics pipeline");
            }

            vkDestroyShaderModule(device, fragShaderModule, nullptr);
            vkDestroyShaderModule(device, vertShaderModule, nullptr);
        }

        VkShaderModule createShaderModule(const std::vector<char> &code) {
            VkShaderModuleCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            createInfo.codeSize = code.size();
            createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

            VkShaderModule shaderModule;
            if (vkCreateShaderModule(device, &createInfo,
                                     nullptr, &shaderModule) != VK_SUCCESS) {
                CLogger::Error("Failed to create shader module");
                throw std::runtime_error("Failed to create shader module");
            }
            return shaderModule;
        }

        //    void createTextureImageView() {
        //        textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB,
        //                                           VK_IMAGE_ASPECT_COLOR_BIT);
        //    }
        //
        //    void createTextureSampler() {
        //        VkSamplerCreateInfo samplerInfo{};
        //        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        //        samplerInfo.magFilter = VK_FILTER_LINEAR;
        //        samplerInfo.minFilter = VK_FILTER_LINEAR;
        //        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        //        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        //        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        //        // if device not supported, set antisotropyEnable to VK_FALSE, maxAntisotropy to 1.0f
        //        samplerInfo.anisotropyEnable = VK_TRUE;
        //        samplerInfo.maxAnisotropy = 16.0f;
        //
        //        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        //        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        //        samplerInfo.compareEnable = VK_FALSE;
        //        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        //        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        //        samplerInfo.mipLodBias = 0.0f;
        //        samplerInfo.minLod = 0.0f;
        //        samplerInfo.maxLod = 0.0f;
        //
        //        if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
        //            CLogger::Error("Failed to create texture sampler");
        //            throw std::runtime_error("Failed to create texture sampler");
        //        }
        //    }
        //
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory) {
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

            if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
                CLogger::Error("Failed to create image");
                throw std::runtime_error("Failed to create image");
            }

            VkMemoryRequirements memRequirements;
            vkGetImageMemoryRequirements(device, image, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
                CLogger::Error("Failed to allocate image memory");
                throw std::runtime_error("Failed to allocate image memory");
            }

            vkBindImageMemory(device, image, imageMemory, 0);
        }

        VkCommandBuffer beginSingleTimeCommands() {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = commandPool;
            allocInfo.commandBufferCount = 1;

            VkCommandBuffer commandBuffer;
            vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(commandBuffer, &beginInfo);

            return commandBuffer;
        }

        void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
            vkEndCommandBuffer(commandBuffer);

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(graphicsQueue);

            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
        }

        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
            VkCommandBuffer commandBuffer = beginSingleTimeCommands();

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
                    commandBuffer,
                    sourceStage, destinationStage,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &barrier);

            endSingleTimeCommands(commandBuffer);
        }

        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
            VkCommandBuffer commandBuffer = beginSingleTimeCommands();

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

            endSingleTimeCommands(commandBuffer);
        }

        //    void createVertexBuffer() {
        //        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
        //
        //        VkBuffer stagingBuffer;
        //        VkDeviceMemory stagingBufferMemory;
        //        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        //                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        //                     stagingBuffer, stagingBufferMemory);
        //
        //        void *data;
        //        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        //        memcpy(data, vertices.data(), (size_t) bufferSize);
        //        vkUnmapMemory(device, stagingBufferMemory);
        //
        //        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        //                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
        //
        //        copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
        //        vkDestroyBuffer(device, stagingBuffer, nullptr);
        //        vkFreeMemory(device, stagingBufferMemory, nullptr);
        //    }
        //
        void createDescriptorSetLayout() {
            VkDescriptorSetLayoutBinding uboLayoutBinding{};
            uboLayoutBinding.binding = 0;
            uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            uboLayoutBinding.descriptorCount = 1;
            uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            uboLayoutBinding.pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutBinding samplerLayoutBinding{};
            samplerLayoutBinding.binding = 1;
            samplerLayoutBinding.descriptorCount = 1;
            samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerLayoutBinding.pImmutableSamplers = nullptr;
            samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

            std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            if (vkCreateDescriptorSetLayout(
                        device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
                CLogger::Error("Failed to create descriptor set layout");
                throw std::runtime_error("Failed to create descriptor set layout");
            }
        }

        //    void createIndexBuffer() {
        //        //            std::vector<uint16_t> indices;
        //
        //        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
        //
        //        VkBuffer stagingBuffer;
        //        VkDeviceMemory stagingBufferMemory;
        //        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        //                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        //                     stagingBuffer, stagingBufferMemory);
        //
        //        void *data;
        //        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        //        memcpy(data, indices.data(), (size_t) bufferSize);
        //        vkUnmapMemory(device, stagingBufferMemory);
        //
        //        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        //                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
        //
        //        copyBuffer(stagingBuffer, indexBuffer, bufferSize);
        //
        //        vkDestroyBuffer(device, stagingBuffer, nullptr);
        //        vkFreeMemory(device, stagingBufferMemory, nullptr);
        //    }
        //
        //    void createUniformBuffers() {
        //        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        //        uniformBuffers.resize(swapChainImages.size());
        //        uniformBuffersMemory.resize(swapChainImages.size());
        //
        //        for (size_t i = 0; i < swapChainImages.size(); i++) {
        //            createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        //                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        //                         uniformBuffers[i], uniformBuffersMemory[i]);
        //        }
        //    }

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
            VkPhysicalDeviceMemoryProperties memProperties;
            vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

            for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
                if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }
            CLogger::Error("Failed to find suitable memory type");
            throw std::runtime_error("Failed to find suitable memory type");
        }

        //    void createDescriptorPool() {
        //        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        //        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        //        poolSizes[0].descriptorCount = static_cast<uint32_t>(swapChainImages.size());
        //        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //        poolSizes[1].descriptorCount = static_cast<uint32_t>(swapChainImages.size());
        //
        //        VkDescriptorPoolCreateInfo poolInfo{};
        //        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        //        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        //        poolInfo.pPoolSizes = poolSizes.data();
        //        poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());
        //
        //        if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        //            CLogger::Error("Failed to create descriptor pool");
        //            throw std::runtime_error("Failed to create descriptor pool");
        //        }
        //    }
        //
        //    void createDescriptorSets() {
        //        std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
        //        VkDescriptorSetAllocateInfo allocInfo{};
        //        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        //        allocInfo.descriptorPool = descriptorPool;
        //        allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
        //        allocInfo.pSetLayouts = layouts.data();
        //
        //        descriptorSets.resize(swapChainImages.size());
        //
        //        if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        //            CLogger::Error("Failed to allocate descriptor sets");
        //            throw std::runtime_error("Failed to allocate descriptor sets");
        //        }
        //
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
        //    }

        void createInstance() {
            if (GlobalPreferences::enableValidationLayers && !checkValidationLayerSupport()) {
                CLogger::Error("Validation layers requested, but not available");
                throw std::runtime_error("Validation layers requested, but not available");
            }

            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = GlobalPreferences::APPLICATION_NAME;

            auto applicationVersion = Assets::Parser::parseStringToSemver(GlobalPreferences::APPLICATION_VERSION);
            auto engineVersion = Assets::Parser::parseStringToSemver(GlobalPreferences::ENGINE_VERSION);

            appInfo.applicationVersion = VK_MAKE_VERSION(applicationVersion.major, applicationVersion.minor, applicationVersion.patch);
            appInfo.pEngineName = GlobalPreferences::ENGINE_NAME;
            appInfo.engineVersion = VK_MAKE_VERSION(engineVersion.major, engineVersion.minor, engineVersion.patch);
            appInfo.apiVersion = VK_API_VERSION_1_0;

            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            auto extensions = getRequiredExtensions();
            createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            createInfo.ppEnabledExtensionNames = extensions.data();

            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
            if (GlobalPreferences::enableValidationLayers) {
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();

                populateDebugMessengerCreateInfo(debugCreateInfo);
                createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
            } else {
                createInfo.enabledLayerCount = 0;
                createInfo.pNext = nullptr;
            }

            if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
                CLogger::Error("Failed to create instance");
                throw std::runtime_error("Failed to create instance");
            }
        }

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
            createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity =
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType =
                    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debugCallback;
        }

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
            SwapChainSupportDetails details;

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

            if (formatCount != 0) {
                details.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(
                        device, surface, &formatCount,
                        details.formats.data());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

            if (presentModeCount != 0) {
                details.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(
                        device, surface, &presentModeCount,
                        details.presentModes.data());
            }

            return details;
        }

        void setupDebugMessenger() {
            if (!GlobalPreferences::enableValidationLayers) return;

            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            populateDebugMessengerCreateInfo(createInfo);

            if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
                CLogger::Error("Failed to set up debug message");
                throw std::runtime_error("Failed to set up debug messenger");
            }
        }

        void createSurface() {
            if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
                CLogger::Error("Failed to create window surface");
                throw std::runtime_error("Failed to create window surface");
            }
        }

        void createFrameBuffers() {
            swapChainFramebuffers.resize(swapChainImageViews.size());

            for (size_t i = 0; i < swapChainImageViews.size(); i++) {
                std::array<VkImageView, 2> attachments = {
                        swapChainImageViews[i],
                        depthImageView};

                VkFramebufferCreateInfo frameBufferInfo{};
                frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                frameBufferInfo.renderPass = renderPass;
                frameBufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
                frameBufferInfo.pAttachments = attachments.data();
                frameBufferInfo.width = swapChainExtent.width;
                frameBufferInfo.height = swapChainExtent.height;
                frameBufferInfo.layers = 1;

                if (vkCreateFramebuffer(
                            device, &frameBufferInfo,
                            nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
                    CLogger::Error("Failed to create FrameBuffer");
                    throw std::runtime_error("Failed to create FrameBuffer");
                }
            }
        }

        void pickPhysicalDevice() {
            uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

            if (deviceCount == 0) {
                CLogger::Error("Failed to find GPUs with Vulkan support");
                throw std::runtime_error("Failed to find GPUs with Vulkan support");
            }

            std::vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

            for (const auto &device : devices) {
                if (isDeviceSuitable(device)) {
                    physicalDevice = device;
                    break;
                }
            }

            if (physicalDevice == VK_NULL_HANDLE) {
                CLogger::Error("Failed to find a suitable CPU");
                throw std::runtime_error("Failed to find a suitable GPU");
            }
        }

        void createLogicalDevice() {
            QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

            std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
            std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

            float queuePriority = 1.0f;
            for (uint32_t queueFamily : uniqueQueueFamilies) {
                VkDeviceQueueCreateInfo queueCreateInfo{};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo.queueFamilyIndex = queueFamily;
                queueCreateInfo.queueCount = 1;
                queueCreateInfo.pQueuePriorities = &queuePriority;
                queueCreateInfos.push_back(queueCreateInfo);
            }

            VkPhysicalDeviceFeatures deviceFeatures{};
            deviceFeatures.samplerAnisotropy = VK_TRUE;

            VkDeviceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

            createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
            createInfo.pQueueCreateInfos = queueCreateInfos.data();

            createInfo.pEnabledFeatures = &deviceFeatures;

            createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
            createInfo.ppEnabledExtensionNames = deviceExtensions.data();

            if (GlobalPreferences::enableValidationLayers) {
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();
            } else {
                createInfo.enabledLayerCount = 0;
            }

            if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
                CLogger::Error("Failed to create logical device");
                throw std::runtime_error("Failed to create logical device");
            }

            vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
            vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
        }

        void createSwapChain() {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
            VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(
                    swapChainSupport.formats);
            VkPresentModeKHR presentMode = chooseSwapPresentMode(
                    swapChainSupport.presentModes);
            VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

            uint32_t imageCount = swapChainSupport.capabilities.minImageCount;

            if (swapChainSupport.capabilities.maxImageCount > 0 &&
                imageCount > swapChainSupport.capabilities.maxImageCount) {
                imageCount = swapChainSupport.capabilities.maxImageCount;
            }

            VkSwapchainCreateInfoKHR createInfo{};

            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = surface;
            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
            uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
                                             indices.presentFamily.value()};

            if (indices.graphicsFamily != indices.presentFamily) {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            } else {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                createInfo.queueFamilyIndexCount = 0;
                createInfo.pQueueFamilyIndices = nullptr;
            }

            createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = presentMode;
            createInfo.clipped = VK_TRUE;
            createInfo.oldSwapchain = VK_NULL_HANDLE;

            if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
                CLogger::Error("Failed to create swap chain");
                throw std::runtime_error("Failed to create swap chain");
            }

            vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
            swapChainImages.resize(imageCount);
            vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

            swapChainImageFormat = surfaceFormat.format;
            swapChainExtent = extent;
        }

        void recreateSwapChain() {
            int width = 0, height = 0;
            while (width == 0 || height == 0) {
                glfwGetFramebufferSize(window, &width, &height);
                glfwWaitEvents();
            }

            vkDeviceWaitIdle(device);

            cleanupSwapChain();
            //VULKAN_DRAW_MANAGER->ReCreateSwapChain();

            createSwapChain();
            createImageViews();
            createRenderPass();
            createGraphicsPipeline();
            createDepthResources();
            createFrameBuffers();
            //        createUniformBuffers();
            //        createDescriptorPool();
            //        createDescriptorSets();
            //        createCommandBuffers();
            //            for (const GraphicsObject graphicsObject : *graphicsObjects) {
            //                std::cout << "FUCFKCUFKCUKF" << std::endl;
            //                auto gObjTexture = graphicsObject.getTexture();
            //                textureImage = *gObjTexture->image;
            //                textureImageMemory = *gObjTexture->deviceMemory;
            //                createFrameBuffers();
            //                createUniformBuffers();
            //                createDescriptorPool();
            //                createDescriptorSets();
            //                createCommandBuffers();
            //            }
        }

        void createImageViews() {
            swapChainImageViews.resize(swapChainImages.size());

            for (uint32_t i = 0; i < swapChainImages.size(); i++) {
                swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
            }
        }

        bool isDeviceSuitable(VkPhysicalDevice device) {
            QueueFamilyIndices indices = findQueueFamilies(device);

            bool extensionsSupported = checkDeviceExtensionSupport(device);

            bool swapChainAdequate = false;
            if (extensionsSupported) {
                SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
                swapChainAdequate = !swapChainSupport.formats.empty() &&
                                    !swapChainSupport.presentModes.empty();
            }

            VkPhysicalDeviceFeatures supportedFeatures;
            vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

            return indices.isComplete() && extensionsSupported &&
                   swapChainAdequate && supportedFeatures.samplerAnisotropy;
        }

        void createRenderPass() {
            VkAttachmentDescription colorAttachment{};
            colorAttachment.format = swapChainImageFormat;
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference colorAttachmentRef{};
            colorAttachmentRef.attachment = 0;
            colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkAttachmentDescription depthAttachment{};
            depthAttachment.format = findDepthFormat();
            depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentReference depthAttachmentRef{};
            depthAttachmentRef.attachment = 1;
            depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass{};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &colorAttachmentRef;
            subpass.pDepthStencilAttachment = &depthAttachmentRef;

            VkSubpassDependency dependency{};
            dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            dependency.dstSubpass = 0;
            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency.srcAccessMask = 0;
            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};

            VkRenderPassCreateInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            renderPassInfo.pAttachments = attachments.data();
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpass;
            renderPassInfo.dependencyCount = 1;
            renderPassInfo.pDependencies = &dependency;


            if (vkCreateRenderPass(device, &renderPassInfo,
                                   nullptr, &renderPass) != VK_SUCCESS) {
                CLogger::Error("Failed to create render pass");
                throw std::runtime_error("Failed to create render pass");
            }
        }

        void createCommandPool() {
            QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

            VkCommandPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
            poolInfo.flags = 0;

            if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
                CLogger::Error("Failed to create command pool");
                throw std::runtime_error("Failed to create command pool");
            }
        }

        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
            for (VkFormat format : candidates) {
                VkFormatProperties props;
                vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
                if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                    return format;
                } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                    return format;
                }
            }

            CLogger::Error("Failed to find supported format");
            throw std::runtime_error("Failed to find supported format");
        }

        VkFormat findDepthFormat() {
            return findSupportedFormat(
                    {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                    VK_IMAGE_TILING_OPTIMAL,
                    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
        }

        bool hasStencilComponent(VkFormat format) {
            return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
                   format == VK_FORMAT_D24_UNORM_S8_UINT;
        }

        void createDepthResources() {
            VkFormat depthFormat = findDepthFormat();

            createImage(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
            depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
        }

        //    void createCommandBuffers() {
        //        commandBuffers.resize(swapChainFramebuffers.size());
        //
        //        VkCommandBufferAllocateInfo allocInfo{};
        //        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        //        allocInfo.commandPool = commandPool;
        //        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        //        allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();
        //
        //        if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        //            CLogger::Error("Failed to allocate command buffers");
        //            throw std::runtime_error("Failed to allocate command buffers");
        //        }
        //
        //        for (size_t i = 0; i < commandBuffers.size(); i++) {
        //            if(i == 0) {
        //                auto textureImageInitial = createTextureImage("texture2.jpg");
        //                textureImage = *textureImageInitial->image;
        //                textureImageMemory = *textureImageInitial->deviceMemory;
        //                createTextureImageView();
        //                for (size_t i = 0; i < swapChainImages.size(); i++) {
        //                    VkDescriptorBufferInfo bufferInfo{};
        //                    bufferInfo.buffer = uniformBuffers[i];
        //                    bufferInfo.offset = 0;
        //                    bufferInfo.range = sizeof(UniformBufferObject);
        //
        //                    VkDescriptorImageInfo imageInfo{};
        //                    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        //                    imageInfo.imageView = textureImageView;
        //                    imageInfo.sampler = textureSampler;
        //                    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
        //
        //                    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        //                    descriptorWrites[0].dstSet = descriptorSets[i];
        //                    descriptorWrites[0].dstBinding = 0;
        //                    descriptorWrites[0].dstArrayElement = 0;
        //                    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        //                    descriptorWrites[0].descriptorCount = 1;
        //                    descriptorWrites[0].pBufferInfo = &bufferInfo;
        //
        //                    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        //                    descriptorWrites[1].dstSet = descriptorSets[i];
        //                    descriptorWrites[1].dstBinding = 1;
        //                    descriptorWrites[1].dstArrayElement = 0;
        //                    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //                    descriptorWrites[1].descriptorCount = 1;
        //                    descriptorWrites[1].pImageInfo = &imageInfo;
        //
        //                    vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        //                }
        //            }
        //
        //            if(i == 2) {
        //                auto textureImageInitial = createTextureImage("texture.jpg");
        //                textureImage = *textureImageInitial->image;
        //                textureImageMemory = *textureImageInitial->deviceMemory;
        //                createTextureImageView();
        //                for (size_t i = 0; i < swapChainImages.size(); i++) {
        //                    VkDescriptorBufferInfo bufferInfo{};
        //                    bufferInfo.buffer = uniformBuffers[i];
        //                    bufferInfo.offset = 0;
        //                    bufferInfo.range = sizeof(UniformBufferObject);
        //
        //                    VkDescriptorImageInfo imageInfo{};
        //                    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        //                    imageInfo.imageView = textureImageView;
        //                    imageInfo.sampler = textureSampler;
        //                    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
        //
        //                    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        //                    descriptorWrites[0].dstSet = descriptorSets[i];
        //                    descriptorWrites[0].dstBinding = 0;
        //                    descriptorWrites[0].dstArrayElement = 0;
        //                    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        //                    descriptorWrites[0].descriptorCount = 1;
        //                    descriptorWrites[0].pBufferInfo = &bufferInfo;
        //
        //                    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        //                    descriptorWrites[1].dstSet = descriptorSets[i];
        //                    descriptorWrites[1].dstBinding = 1;
        //                    descriptorWrites[1].dstArrayElement = 0;
        //                    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //                    descriptorWrites[1].descriptorCount = 1;
        //                    descriptorWrites[1].pImageInfo = &imageInfo;
        //
        //                    vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        //                }
        //            }
        //
        //
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
        //            renderPassInfo.renderPass = renderPass;
        //            renderPassInfo.framebuffer = swapChainFramebuffers[i];
        //            renderPassInfo.renderArea.offset = {0, 0};
        //            renderPassInfo.renderArea.extent = swapChainExtent;
        //
        //            std::array<VkClearValue, 2> clearValues{};
        //            clearValues[0] = {0.0f, 0.0f, 0.0f, 1.0f};
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
        //                              graphicsPipeline);
        //
        //            VkBuffer vertexBuffers[] = {vertexBuffer};
        //            VkDeviceSize offsets[] = {0};
        //            vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
        //
        //            vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
        //
        //            vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
        //                                    pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
        //
        //            vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
        //
        //            vkCmdEndRenderPass(commandBuffers[i]);
        //
        //            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
        //                CLogger::Error("Failed to record command buffer");
        //                throw std::runtime_error("Failed to record command buffer");
        //            }
        //        }
        //    }

        bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(
                    device, nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);

            vkEnumerateDeviceExtensionProperties(
                    device, nullptr, &extensionCount,
                    availableExtensions.data());

            std::set<std::string> requiredExtensions(
                    deviceExtensions.begin(),
                    deviceExtensions.end());

            for (const auto &extension : availableExtensions) {
                requiredExtensions.erase(extension.extensionName);
            }

            return requiredExtensions.empty();
        }

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
            QueueFamilyIndices indices;

            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            int i = 0;
            for (const auto &queueFamily : queueFamilies) {
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    indices.graphicsFamily = i;
                }

                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

                if (presentSupport) {
                    indices.presentFamily = i;
                }

                if (indices.isComplete()) {
                    break;
                }

                i++;
            }

            return indices;
        }

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(
                const std::vector<VkSurfaceFormatKHR> &availableFormats) {
            for (const auto &availableFormat : availableFormats) {
                if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                    availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    return availableFormat;
                }
            }

            return availableFormats[0];
        }

        VkPresentModeKHR chooseSwapPresentMode(
                const std::vector<VkPresentModeKHR> &availablePresentModes) {
            for (const auto &availablePresentMode : availablePresentModes) {
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return availablePresentMode;
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
            if (capabilities.currentExtent.width != UINT32_MAX) {
                return capabilities.currentExtent;
            } else {
                int width, height;
                glfwGetFramebufferSize(window, &width, &height);
                VkExtent2D actualExtent = {GlobalPreferences::SCREEN_WIDTH,
                                           GlobalPreferences::SCREEN_WIDTH};

                actualExtent.width = std::max(
                        capabilities.minImageExtent.width,
                        std::min(capabilities.maxImageExtent.width,
                                 actualExtent.width));
                actualExtent.height = std::max(
                        capabilities.minImageExtent.height,
                        std::min(capabilities.maxImageExtent.height,
                                 actualExtent.height));
                return actualExtent;
            }
        }

        std::vector<const char *> getRequiredExtensions() {
            uint32_t glfwExtensionCount = 0;
            const char **glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            if (GlobalPreferences::enableValidationLayers) {
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return extensions;
        }

        bool checkValidationLayerSupport() {
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            for (const char *layerName : validationLayers) {
                bool layerFound = false;

                for (const auto &layerProperties : availableLayers) {
                    if (strcmp(layerName, layerProperties.layerName) == 0) {
                        layerFound = true;
                        break;
                    }
                }
                if (!layerFound) {
                    return false;
                }
            }
            return true;
        };

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT
                                                                    *pCallbackData,
                                                            void *pUserData) {
            CLogger::Debug("[VK] %s", pCallbackData->pMessage);
            return VK_FALSE;
        }
    };
};// namespace VulkanCore