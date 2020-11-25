//
// Created by HADMARINE on 2020/11/24.
//

#ifndef SHARKENGINE_SHARKVULKANDEFINE_HPP
#define SHARKENGINE_SHARKVULKANDEFINE_HPP

#define SHARK_API_CORE SharkEngine::Core::SharkVulkan::GetCore()
#define SHARK_API_DEVICE SHARK_API_CORE->GetDevice()

#include "IncludeVulkan.hpp"

namespace SharkEngine::Core {

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

    struct TextureImageStruct {
        VkImage *image;
        VkDeviceMemory *deviceMemory;
    };

    class GraphicsObject {
    private:
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
        TextureImageStruct *texture;
    public:
        static int indexCount;
        std::vector<Vertex> getVertices() { return vertices; };
        std::vector<uint16_t> getIndices() { return indices; };
        TextureImageStruct *getTexture() const { return texture; };
        GraphicsObject(const std::vector<Vertex> &vertices, TextureImageStruct *texture) {
            this->vertices = vertices;
            this->texture = texture;
            for (int i = 0; i < vertices.size() - 2; i++) {
                this->indices.push_back(indexCount);
                this->indices.push_back(indexCount + i + 1);
                this->indices.push_back(indexCount + i + 2);
            }
            indexCount += vertices.size();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkDebugUtilsMessengerEXT* pDebugMessenger);

    void DestroyDebugUtilsMessengerdEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks* pAllocator);

    struct BufferCreateInfo {
        VkBuffer buffer;
        VkDeviceMemory memory;
    };

}

#endif//SHARKENGINE_SHARKVULKANDEFINE_HPP
