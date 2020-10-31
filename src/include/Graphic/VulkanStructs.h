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

extern std::vector<const char *> instanceExtensionNames;
extern std::vector<const char *> layerNames;
extern std::vector<const char *> deviceExtensionNames;

#endif//SHARKENGINE_WRAPPER_H