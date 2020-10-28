//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_WRAPPER_H
#define SHARKENGINE_WRAPPER_H

#include "IncludeVulkan.h"
/***************COMMAND BUFFER WRAPPERS***************/
class CommandBufferMgr
{
public:
    static void allocCommandBuffer(const VkDevice* device, const VkCommandPool cmdPool, VkCommandBuffer* cmdBuf, const VkCommandBufferAllocateInfo* commandBufferInfo = NULL);
    static void beginCommandBuffer(VkCommandBuffer cmdBuf, VkCommandBufferBeginInfo* inCmdBufInfo = NULL);
    static void endCommandBuffer(VkCommandBuffer cmdBuf);
    static void submitCommandBuffer(const VkQueue& queue, const VkCommandBuffer* cmdBufList, const VkSubmitInfo* submitInfo = NULL, const VkFence& fence = VK_NULL_HANDLE);
};

void* readFile(const char *spvFileName, size_t *fileSize);

/***************TEXTURE WRAPPERS***************/
struct TextureData{
    VkSampler				sampler;
    VkImage					image;
    VkImageLayout			imageLayout;
    VkMemoryAllocateInfo	memoryAlloc;
    VkDeviceMemory			mem;
    VkImageView				view;
    uint32_t				mipMapLevels;
    uint32_t				layerCount;
    uint32_t				textureWidth, textureHeight;
    VkDescriptorImageInfo	descsImgInfo;
};

/***************DRAWABLE WRAPPERS***************/
struct UniformData {
    VkBuffer						buffer;			// Buffer resource object
    VkDeviceMemory					memory;			// Buffer resourece object's allocated device memory
    VkDescriptorBufferInfo			bufferInfo;		// Buffer info that need to supplied into write descriptor set (VkWriteDescriptorSet)
    VkMemoryRequirements			memRqrmnt;		// Store the queried memory requirement of the uniform buffer
    std::vector<VkMappedMemoryRange>mappedRange;	// Metadata of memory mapped objects
    uint8_t*						pData;			// Host pointer containing the mapped device address which is used to write data into.
} UniformData;

// Structure storing vertex buffer metadata
struct VertexBuffer {
    VkBuffer buf;
    VkDeviceMemory mem;
    VkDescriptorBufferInfo bufferInfo;
} VertexBuffer;

/***************PPM PARSER CLASS***************/

class PpmParser
{
public:
    PpmParser();
    ~PpmParser();
    bool getHeaderInfo(const char *filename);
    bool loadImageData(int rowPitch, uint8_t *rgba_data);
    int32_t getImageWidth();
    int32_t getImageHeight();
    const char* filename() { return ppmFile.c_str(); }

private:
    bool isValid;
    int32_t imageWidth;
    int32_t imageHeight;
    int32_t dataPosition;
    std::string ppmFile;
    gli::texture2d* tex2D;
};

struct SwapChainBuffer{
    VkImage image;
    VkImageView view;
};

/*************** SWAP CHAIN STRUCTURE **************/
struct SwapChainPrivateVariables
{
    // Store the image surface capabilities
    VkSurfaceCapabilitiesKHR	surfCapabilities;

    // Stores the number of present mode support by the implementation
    uint32_t					presentModeCount;

    // Arrays for retrived present modes
    std::vector<VkPresentModeKHR> presentModes;

    // Size of the swap chain color images
    VkExtent2D					swapChainExtent;

    // Number of color images supported by the implementation
    uint32_t					desiredNumberOfSwapChainImages;

    VkSurfaceTransformFlagBitsKHR preTransform;

    // Stores present mode bitwise flag for the creation of swap chain
    VkPresentModeKHR			swapchainPresentMode;

    // The retrived drawing color swap chain images
    std::vector<VkImage>		swapchainImages;

    std::vector<VkSurfaceFormatKHR> surfFormats;
};

struct SwapChainPublicVariables
{
    // The logical platform dependent surface object
    VkSurfaceKHR surface;

    // Number of buffer image used for swap chain
    uint32_t swapchainImageCount;

    // Swap chain object
    VkSwapchainKHR swapChain;

    // List of color swap chain images
    std::vector<SwapChainBuffer> colorBuffer;

    // Semaphore for sync purpose
    VkSemaphore presentCompleteSemaphore;

    // Current drawing surface index in use
    uint32_t currentColorBuffer;

    // Format of the image
    VkFormat format;
};

#endif//SHARKENGINE_WRAPPER_H