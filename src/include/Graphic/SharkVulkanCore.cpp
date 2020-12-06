//
// Created by HADMARINE on 2020/11/24.
//

#include "SharkVulkan.hpp"

namespace SharkEngine::Core {

    void SharkVulkan::Init() {
        InitWindow();
        InitVulkan();
    }

    void SharkVulkan::Run() {
        MainLoop();
        Cleanup();
    }

    void SharkVulkan::InitWindow() {
        glfwInit();

        // GLFW SETTINGS
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(
                GlobalPreferences::SCREEN_WIDTH,
                GlobalPreferences::SCREEN_HEIGHT,
                GlobalPreferences::APPLICATION_NAME,
                nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, SharkVulkan::FramebufferResizeCallback);
    }

    void SharkVulkan::MainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            DrawFrame();
        }
        vkDeviceWaitIdle(device);
    }

    void SharkVulkan::Cleanup() {
    }

    void SharkVulkan::InitVulkan() {
        CreateInstance();
        SetupDebugMessenger();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateDescriptorSetLayout();
        CreateGraphicsPipeline();
        CreateCommandPool();
        CreateDepthResources();
        CreateFrameBuffers();
        CreateTextureSampler();
        CreateUniformBuffers();
        CreateDescriptorPool();
        CreateCommandBuffers();
        CreateSyncObjects();
    }

}// namespace SharkEngine::Core