//
// Created by EunwooSong on 2020-10-27.
//

#include "../../include/Graphic/VulkanStructs.h"

// PPM parser implementation
PpmParser::PpmParser() {
    isValid			= false;
    imageWidth		= 0;
    imageHeight		= 0;
    ppmFile			= "invalid file name";
    dataPosition	= 0;
}
PpmParser::~PpmParser() {
}
bool PpmParser::getHeaderInfo(const char *filename) {
    tex2D = new gli::texture2d(gli::load(filename));
    imageHeight = static_cast<uint32_t>(tex2D[0].extent().x);
    imageWidth  = static_cast<uint32_t>(tex2D[0].extent().y);
    return true;
}
bool PpmParser::loadImageData(int rowPitch, uint8_t *data) {
    uint8_t* dataTemp = (uint8_t*)tex2D->data();
    for (int y = 0; y < imageHeight; y++)
    {
        size_t imageSize = imageWidth * 4;
        memcpy(data, dataTemp, imageSize);
        dataTemp += imageSize;

        // Advance row by row pitch information
        data += rowPitch;
    }

    return true;
}
int32_t PpmParser::getImageWidth() {
    return 0;
}
int32_t PpmParser::getImageHeight() {
    return 0;
}
void CommandBufferMgr::allocCommandBuffer(const VkDevice *device, const VkCommandPool cmdPool, VkCommandBuffer *cmdBuf, const VkCommandBufferAllocateInfo *commandBufferInfo) {
    VkResult result;

    if (commandBufferInfo) {
        result = vkAllocateCommandBuffers(*device, commandBufferInfo, cmdBuf);
        assert(!result);
        return;
    }

    VkCommandBufferAllocateInfo cmdInfo = {};
    cmdInfo.sType		= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdInfo.pNext		= NULL;
    cmdInfo.commandPool = cmdPool;
    cmdInfo.level		= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdInfo.commandBufferCount = (uint32_t) sizeof(cmdBuf) / sizeof(VkCommandBuffer);;

    result = vkAllocateCommandBuffers(*device, &cmdInfo, cmdBuf);
}
void CommandBufferMgr::beginCommandBuffer(VkCommandBuffer cmdBuf, VkCommandBufferBeginInfo *inCmdBufInfo) {
    VkResult  result;
    // If the user has specified the custom command buffer use it
    if (inCmdBufInfo) {
        result = vkBeginCommandBuffer(cmdBuf, inCmdBufInfo);
        assert(result == VK_SUCCESS);
        return;
    }

    // Otherwise, use the default implementation.
    VkCommandBufferInheritanceInfo cmdBufInheritInfo = {};
    cmdBufInheritInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    cmdBufInheritInfo.pNext					= NULL;
    cmdBufInheritInfo.renderPass			= VK_NULL_HANDLE;
    cmdBufInheritInfo.subpass				= 0;
    cmdBufInheritInfo.framebuffer			= VK_NULL_HANDLE;
    cmdBufInheritInfo.occlusionQueryEnable	= VK_FALSE;
    cmdBufInheritInfo.queryFlags			= 0;
    cmdBufInheritInfo.pipelineStatistics	= 0;

    VkCommandBufferBeginInfo cmdBufInfo = {};
    cmdBufInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufInfo.pNext				= NULL;
    cmdBufInfo.flags				= 0;
    cmdBufInfo.pInheritanceInfo		= &cmdBufInheritInfo;

    result = vkBeginCommandBuffer(cmdBuf, &cmdBufInfo);
}
void CommandBufferMgr::endCommandBuffer(VkCommandBuffer cmdBuf) {
    VkResult  result;
    result = vkEndCommandBuffer(cmdBuf);
    assert(result == VK_SUCCESS);
}
void CommandBufferMgr::submitCommandBuffer(VkQueue const &queue, const VkCommandBuffer *cmdBufList, const VkSubmitInfo *inSubmitInfo, const VkFence &fence) {
    VkResult result;

    if (inSubmitInfo){
        result = vkQueueSubmit(queue, 1, inSubmitInfo, fence);
        assert(!result);

        result = vkQueueWaitIdle(queue);
        assert(!result);
        return;
    }

    // Otherwise, create the submit information with specified buffer commands
    VkSubmitInfo submitInfo = {};
    submitInfo.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext				= NULL;
    submitInfo.waitSemaphoreCount	= 0;
    submitInfo.pWaitSemaphores		= NULL;
    submitInfo.pWaitDstStageMask	= NULL;
    submitInfo.commandBufferCount	= (uint32_t)sizeof(cmdBufList)/sizeof(VkCommandBuffer);
    submitInfo.pCommandBuffers		= cmdBufList;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores	= NULL;

    result = vkQueueSubmit(queue, 1, &submitInfo, fence);

    result = vkQueueWaitIdle(queue);
}

void* readFile(const char *spvFileName, size_t *fileSize) {

    FILE *fp = fopen(spvFileName, "rb");
    if (!fp) {
        return NULL;
    }

    size_t retval;
    long int size;

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);

    void* spvShader = malloc(size+1); // Plus for NULL character '\0'
    memset(spvShader, 0, size+1);

    retval = fread(spvShader, size, 1, fp);
    assert(retval == 1);

    *fileSize = size;
    fclose(fp);
    return spvShader;
}