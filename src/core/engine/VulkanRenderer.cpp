//
// Created by EunwooSong on 2020-10-27.
//

#include "../../include/Graphic/VulkanRenderer.h"

#include "../../include/Graphic/VulkanApplication.h"
#include "../../include/Graphic/VulkanDevice.h"
#include "../../include/Graphic/VulkanMeshStruct.h"

VulkanRenderer::VulkanRenderer(VulkanApplication *app, VulkanDevice *deviceObject) {
    memset(&Depth, 0, sizeof(Depth));
    memset(&connection, 0, sizeof(HINSTANCE));				// hInstance - Windows Instance

    application = app;
    deviceObj	= deviceObject;

    swapChainObj = new VulkanSwapChain(this);
    VulkanDrawable* drawableObj = new VulkanDrawable(this);
    drawableList.push_back(drawableObj);
}
VulkanRenderer::~VulkanRenderer() {
    delete swapChainObj;
    swapChainObj = NULL;
    for(auto d : drawableList)
    {
        delete d;
    }
    drawableList.clear();
}
void VulkanRenderer::Initialize() {
    CreateCommandPool();

    BuildSwapChainAndDepthImage();

    CreateVertexBuffer();

    const bool includeDepth = true;
    // Create the render pass now . . .
    CreateRenderPass(includeDepth);

    // Use render pass and create frame buffer
    CreateFrameBuffer(includeDepth);

    CreateShaders();

    CreateTextureLinear("../../source/texture.jpg", VK_IMAGE_USAGE_SAMPLED_BIT);
    CreateTextureLinear("../../source/texture2.jpg", VK_IMAGE_USAGE_SAMPLED_BIT);

//    for (VulkanDrawable* drawableObj : drawableList) {
//        drawableObj->SetTextures(&texture);
//    }

    CreateDescriptors();
    CreatePipelineStateManagement();
}
void VulkanRenderer::Prepare() {
    for(VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->Prepare();
    }
}
void VulkanRenderer::Update() {
    for(VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->Update();
    }
}
bool VulkanRenderer::Render() {
    MSG msg;   // message
    PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    if (msg.message == WM_QUIT) {
        return false;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    RedrawWindow(window, NULL, NULL, RDW_INTERNALPAINT);
    return true;
}

#ifdef _WIN32
LRESULT VulkanRenderer::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    VulkanApplication* appObj = VulkanApplication::GetInstance();
    switch (uMsg) {

        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_PAINT:
            for (VulkanDrawable* drawableObj : appObj->rendererObj->drawableList)
            {
                drawableObj->Render();
            }

            return 0;

        case WM_SIZE:
            if (wParam != SIZE_MINIMIZED) {
                appObj->rendererObj->width = lParam & 0xffff;
                appObj->rendererObj->height = (lParam & 0xffff0000) >> 16;
                appObj->rendererObj->GetSwapChain()->SetSwapChainExtent(appObj->rendererObj->width, appObj->rendererObj->height);
                appObj->Resize();
            }
            break;

        default:
            break;
    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}
void VulkanRenderer::CreatePresentationWindow(const int &windowWidth, const int &windowHeight) {
#ifdef _WIN32
    width	= windowWidth;
    height	= windowHeight;
    assert(width > 0 || height > 0);

    WNDCLASSEX  winInfo;

    sprintf(name, "Texture demo - Linear Layout");
    memset(&winInfo, 0, sizeof(WNDCLASSEX));
    // Initialize the window class structure:
    winInfo.cbSize			= sizeof(WNDCLASSEX);
    winInfo.style			= CS_HREDRAW | CS_VREDRAW;
    winInfo.lpfnWndProc		= WndProc;
    winInfo.cbClsExtra		= 0;
    winInfo.cbWndExtra		= 0;
    winInfo.hInstance		= connection; // hInstance
    winInfo.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
    winInfo.hCursor			= LoadCursor(NULL, IDC_ARROW);
    winInfo.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
    winInfo.lpszMenuName	= NULL;
    winInfo.lpszClassName	= name;
    winInfo.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);

    // Register window class:
    if (!RegisterClassEx(&winInfo)) {
        // It didn't work, so try to give a useful error:
        CLogger::Error("Unexpected error trying to start the application!\n");
        exit(1);
    }

    // Create window with the registered class:
    RECT wr = { 0, 0, width, height };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    window = CreateWindowEx(0,
                            name,					// class name
                            name,					// app name
                            WS_OVERLAPPEDWINDOW |	// window style
                            WS_VISIBLE |
                            WS_SYSMENU,
                            100, 100,				// x/y coords
                            wr.right - wr.left,     // width
                            wr.bottom - wr.top,     // height
                            NULL,					// handle to parent
                            NULL,					// handle to menu
                            connection,				// hInstance
                            NULL);					// no extra parameters

    if (!window) {
        // It didn't work, so try to give a useful error:
        printf("Cannot create a window in which to draw!\n");
        fflush(stdout);
        exit(1);
    }

    SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)&application);
#else
    const xcb_setup_t *setup;
	xcb_screen_iterator_t iter;
	int scr;

	connection = xcb_connect(NULL, &scr);
	if (connection == NULL) {
		std::cout << "Cannot find a compatible Vulkan ICD.\n";
		exit(-1);
	}

	setup = xcb_get_setup(connection);
	iter = xcb_setup_roots_iterator(setup);
	while (scr-- > 0)
		xcb_screen_next(&iter);

	screen = iter.data;
#endif // _WIN32
}

void VulkanRenderer::DestroyPresentationWindow() {
    DestroyWindow(window);
}
#else
void VulkanRenderer::CreatePresentationWindow()
{
	uint32_t value_mask, value_list[32];

	window = xcb_generate_id(connection);

	value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	value_list[0] = screen->black_pixel;
	value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_EXPOSURE;

	xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root, 0, 0, width, height, 0,
		XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, value_mask, value_list);

	/* Magic code that will send notification when window is destroyed */
	xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection, 1, 12, "WM_PROTOCOLS");
	xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(connection, cookie, 0);

	xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(connection, 0, 16, "WM_DELETE_WINDOW");
	reply = xcb_intern_atom_reply(connection, cookie2, 0);

	xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, (*reply).atom, 4, 32, 1,	&(*reply).atom);
	free(reply);

	xcb_map_window(connection, window);

	const uint32_t coords[] = { 100,  100 };
	xcb_configure_window(connection, window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, coords);
	xcb_flush(connection);

	xcb_generic_event_t *e;
	while ((e = xcb_wait_for_event(connection))) {
		if ((e->response_type & ~0x80) == XCB_EXPOSE)
			break;
	}
}

void VulkanRenderer::DestroyWindow()
{
	xcb_destroy_window(connection, window);
	xcb_disconnect(connection);
}
#endif // _WIN32
void VulkanRenderer::SetImageLayout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, const VkImageSubresourceRange &subresourceRange, VkCommandBuffer const &cmdBuf) {
    VkImageMemoryBarrier imgMemoryBarrier = {};
    imgMemoryBarrier.sType			= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imgMemoryBarrier.pNext			= NULL;
    imgMemoryBarrier.srcAccessMask	= 0;
    imgMemoryBarrier.dstAccessMask	= 0;
    imgMemoryBarrier.oldLayout		= oldImageLayout;
    imgMemoryBarrier.newLayout		= newImageLayout;
    imgMemoryBarrier.image			= image;
    imgMemoryBarrier.subresourceRange = subresourceRange;

    if (oldImageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        imgMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }

    // Source layouts (old)
    switch (oldImageLayout)
    {
        case VK_IMAGE_LAYOUT_UNDEFINED:
            imgMemoryBarrier.srcAccessMask = 0;
            break;
        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            imgMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imgMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imgMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }

    switch (newImageLayout)
    {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
            imgMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            imgMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
            imgMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imgMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            imgMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
            break;
    }

    VkPipelineStageFlags srcStages	= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags destStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    vkCmdPipelineBarrier(cmdBuf, srcStages, destStages, 0, 0, NULL, 0, NULL, 1, &imgMemoryBarrier);

}
void VulkanRenderer::CreateCommandPool() {
    VulkanDevice* deviceObj		= application->deviceObj;
    /* Depends on intializeSwapChainExtension() */
    VkResult  res;

    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.pNext = NULL;
    cmdPoolInfo.queueFamilyIndex = deviceObj->graphicsQueueWithPresentIndex;
    cmdPoolInfo.flags = 0;

    res = vkCreateCommandPool(deviceObj->device, &cmdPoolInfo, NULL, &cmdPool);
}
void VulkanRenderer::BuildSwapChainAndDepthImage() {
    deviceObj->GetDeviceQueue();

    swapChainObj->CreateSwapChain(cmdDepthImage);

    CreateDepthImage();
}
void VulkanRenderer::CreateDepthImage() {
    VkResult  result;
    bool  pass;

    VkImageCreateInfo imageInfo = {};

    // If the depth format is undefined, use fallback as 16-byte value
    if (Depth.format == VK_FORMAT_UNDEFINED) {
        Depth.format = VK_FORMAT_D16_UNORM;
    }

    const VkFormat depthFormat = Depth.format;

    imageInfo.sType			= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext			= NULL;
    imageInfo.imageType		= VK_IMAGE_TYPE_2D;
    imageInfo.format		= depthFormat;
    imageInfo.extent.width	= width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth	= 1;
    imageInfo.mipLevels		= 1;
    imageInfo.arrayLayers	= 1;
    imageInfo.samples		= NUM_SAMPLES;
    imageInfo.tiling		= VK_IMAGE_TILING_OPTIMAL;
    imageInfo.queueFamilyIndexCount = 0;
    imageInfo.pQueueFamilyIndices	= NULL;
    imageInfo.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.usage					= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.flags					= 0;

    // User create image info and create the image objects
    result = vkCreateImage(deviceObj->device, &imageInfo, NULL, &Depth.image);
    assert(result == VK_SUCCESS);

    // Get the image memory requirements
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(deviceObj->device,	Depth.image, &memRequirements);

    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloc.pNext = NULL;
    memAlloc.allocationSize = 0;
    memAlloc.memoryTypeIndex = 0;
    memAlloc.allocationSize = memRequirements.size;
    // Determine the type of memory required with the help of memory properties
    pass = deviceObj->MemoryTypeFromProperties(memRequirements.memoryTypeBits, 0, &memAlloc.memoryTypeIndex);

    // Allocate the memory for image objects
    result = vkAllocateMemory(deviceObj->device, &memAlloc, NULL, &Depth.mem);

    // Bind the allocated memeory
    result = vkBindImageMemory(deviceObj->device, Depth.image, Depth.mem, 0);


    VkImageViewCreateInfo imgViewInfo = {};
    imgViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imgViewInfo.pNext = NULL;
    imgViewInfo.image = VK_NULL_HANDLE;
    imgViewInfo.format = depthFormat;
    imgViewInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY };
    imgViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    imgViewInfo.subresourceRange.baseMipLevel = 0;
    imgViewInfo.subresourceRange.levelCount = 1;
    imgViewInfo.subresourceRange.baseArrayLayer = 0;
    imgViewInfo.subresourceRange.layerCount = 1;
    imgViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imgViewInfo.flags = 0;

    if (depthFormat == VK_FORMAT_D16_UNORM_S8_UINT ||
        depthFormat == VK_FORMAT_D24_UNORM_S8_UINT ||
        depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT) {
        imgViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    // Use command buffer to create the depth image. This includes -
    // Command buffer allocation, recording with begin/end scope and submission.
    CommandBufferMgr::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdDepthImage);
    CommandBufferMgr::beginCommandBuffer(cmdDepthImage);
    {
        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = 1;
        subresourceRange.layerCount = 1;

        SetImageLayout(Depth.image,
                       imgViewInfo.subresourceRange.aspectMask,
                       VK_IMAGE_LAYOUT_UNDEFINED,
                       VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, subresourceRange, cmdDepthImage);
    }
    CommandBufferMgr::endCommandBuffer(cmdDepthImage);
    CommandBufferMgr::submitCommandBuffer(deviceObj->queue, &cmdDepthImage);

    imgViewInfo.image = Depth.image;
    result = vkCreateImageView(deviceObj->device, &imgViewInfo, NULL, &Depth.view);
    assert(result == VK_SUCCESS);
}
void VulkanRenderer::CreateVertexBuffer() {
    CommandBufferMgr::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdVertexBuffer);
    CommandBufferMgr::beginCommandBuffer(cmdVertexBuffer);

    for (VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->CreateVertexBuffer(geometryData, sizeof(geometryData), sizeof(geometryData[0]), false);
    }
    CommandBufferMgr::endCommandBuffer(cmdVertexBuffer);
    CommandBufferMgr::submitCommandBuffer(deviceObj->queue, &cmdVertexBuffer);

}
void VulkanRenderer::CreateRenderPass(bool includeDepth, bool clear) {
    VkResult  result;
    // Attach the color buffer and depth buffer as an attachment to render pass instance
    VkAttachmentDescription attachments[2];
    attachments[0].format					= swapChainObj->scPublicVars.format;
    attachments[0].samples					= NUM_SAMPLES;
    attachments[0].loadOp					= clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].storeOp					= VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp			= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp			= VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout			= VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout				= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachments[0].flags					= VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;

    // Is the depth buffer present the define attachment properties for depth buffer attachment.
    if (includeDepth)
    {
        attachments[1].format				= Depth.format;
        attachments[1].samples				= NUM_SAMPLES;
        attachments[1].loadOp				= clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[1].storeOp				= VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].stencilLoadOp		= VK_ATTACHMENT_LOAD_OP_LOAD;
        attachments[1].stencilStoreOp		= VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].initialLayout		= VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[1].finalLayout			= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachments[1].flags				= VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
    }

    // Define the color buffer attachment binding point and layout information
    VkAttachmentReference colorReference	= {};
    colorReference.attachment				= 0;
    colorReference.layout					= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Define the depth buffer attachment binding point and layout information
    VkAttachmentReference depthReference = {};
    depthReference.attachment				= 1;
    depthReference.layout					= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Specify the attachments - color, depth, resolve, preserve etc.
    VkSubpassDescription subpass			= {};
    subpass.pipelineBindPoint				= VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags							= 0;
    subpass.inputAttachmentCount			= 0;
    subpass.pInputAttachments				= NULL;
    subpass.colorAttachmentCount			= 1;
    subpass.pColorAttachments				= &colorReference;
    subpass.pResolveAttachments				= NULL;
    subpass.pDepthStencilAttachment			= includeDepth ? &depthReference : NULL;
    subpass.preserveAttachmentCount			= 0;
    subpass.pPreserveAttachments			= NULL;

    // Specify the attachement and subpass associate with render pass
    VkRenderPassCreateInfo rpInfo			= {};
    rpInfo.sType							= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpInfo.pNext							= NULL;
    rpInfo.attachmentCount					= includeDepth ? 2 : 1;
    rpInfo.pAttachments						= attachments;
    rpInfo.subpassCount						= 1;
    rpInfo.pSubpasses						= &subpass;
    rpInfo.dependencyCount					= 0;
    rpInfo.pDependencies					= NULL;

    // Create the render pass object
    result = vkCreateRenderPass(deviceObj->device, &rpInfo, NULL, &renderPass);

}
void VulkanRenderer::CreateFrameBuffer(bool includeDepth) {
    VkResult  result;
    VkImageView attachments[2];
    attachments[1] = Depth.view;

    VkFramebufferCreateInfo fbInfo	= {};
    fbInfo.sType					= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbInfo.pNext					= NULL;
    fbInfo.renderPass				= renderPass;
    fbInfo.attachmentCount			= includeDepth ? 2 : 1;
    fbInfo.pAttachments				= attachments;
    fbInfo.width					= width;
    fbInfo.height					= height;
    fbInfo.layers					= 1;

    uint32_t i;

    framebuffers.clear();
    framebuffers.resize(swapChainObj->scPublicVars.swapchainImageCount);
    for (i = 0; i < swapChainObj->scPublicVars.swapchainImageCount; i++) {
        attachments[0] = swapChainObj->scPublicVars.colorBuffer[i].view;
        result = vkCreateFramebuffer(deviceObj->device, &fbInfo, NULL, &framebuffers.at(i));
        assert(result == VK_SUCCESS);
    }
}
void VulkanRenderer::CreateShaders() {
    if (application->isResizing)
        return;

    void* vertShaderCode, *fragShaderCode;
    size_t sizeVert, sizeFrag;

#ifdef AUTO_COMPILE_GLSL_TO_SPV
    vertShaderCode = readFile("./../Texture.vert", &sizeVert);
	fragShaderCode = readFile("./../Texture.frag", &sizeFrag);

	shaderObj.buildShader((const char*)vertShaderCode, (const char*)fragShaderCode);
#else
    vertShaderCode = readFile("../src/shaders/vert.spv", &sizeVert);
    fragShaderCode = readFile("../src/shaders/frag.spv", &sizeFrag);

    shaderObj.BuildShaderModuleWithSPV((uint32_t*)vertShaderCode, sizeVert, (uint32_t*)fragShaderCode, sizeFrag);
#endif
}
void VulkanRenderer::CreatePipelineStateManagement() {
    for (VulkanDrawable* drawableObj : drawableList)
    {
        // Use the descriptor layout and create the pipeline layout.
        drawableObj->CreatePipelineLayout();
    }

    pipelineObj.CreatePipelineCache();

    const bool depthPresent = true;
    for (VulkanDrawable* drawableObj : drawableList)
    {
        VkPipeline* pipeline = (VkPipeline*)malloc(sizeof(VkPipeline));
        if (pipelineObj.CreatePipeline(drawableObj, pipeline, &shaderObj, depthPresent))
        {
            pipelineList.push_back(pipeline);
            drawableObj->SetPipeline(pipeline);
        }
        else
        {
            free(pipeline);
            pipeline = NULL;
        }
    }
}
void VulkanRenderer::CreateDescriptors() {
    for (VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->CreateDescriptorSetLayout(true);

        // Create the descriptor set
        drawableObj->CreateDescriptor(true);
    }
}
void VulkanRenderer::CreateTextureLinear(const char *filename, VkImageUsageFlags imageUsageFlags, VkFormat format) {
    if(texturesData[filename])
        return;

    // Load the image
    gli::texture2d image2D(gli::load(filename));
    CLogger::Error("Texture Is Null");
    TextureData* texture = new TextureData();

    texture->textureWidth = static_cast<uint32_t>(image2D[0].extent().x);
    texture->textureWidth = static_cast<uint32_t>(image2D[0].extent().y);
    texture->mipMapLevels = static_cast<uint32_t>(image2D.levels());

    // Create image resource states using VkImageCreateInfo
    VkImageCreateInfo imageCreateInfo   = {};
    imageCreateInfo.sType				= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext				= NULL;
    imageCreateInfo.imageType			= VK_IMAGE_TYPE_2D;
    imageCreateInfo.format				= format;
    imageCreateInfo.extent.width		= texture->textureWidth;
    imageCreateInfo.extent.height		= texture->textureHeight;
    imageCreateInfo.extent.depth		= 1;
    imageCreateInfo.mipLevels			= texture->mipMapLevels;
    imageCreateInfo.arrayLayers			= 1;
    imageCreateInfo.samples				= VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.queueFamilyIndexCount	= 0;
    imageCreateInfo.pQueueFamilyIndices	= NULL;
    imageCreateInfo.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.usage				= imageUsageFlags;
    imageCreateInfo.flags				= 0;
    imageCreateInfo.initialLayout		= VK_IMAGE_LAYOUT_PREINITIALIZED,
            imageCreateInfo.tiling				= VK_IMAGE_TILING_LINEAR;

    VkResult  error;
    // Use create image info and create the image objects
    error = vkCreateImage(deviceObj->device, &imageCreateInfo, NULL, &texture->image);
    assert(!error);

    // Get the buffer memory requirements
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(deviceObj->device, texture->image, &memoryRequirements);

    // Create memory allocation metadata information
    VkMemoryAllocateInfo& memAlloc	= texture->memoryAlloc;
    memAlloc.sType					= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloc.pNext					= NULL;
    memAlloc.allocationSize			= memoryRequirements.size;
    memAlloc.memoryTypeIndex		= 0;

    // Determine the type of memory required
    // with the help of memory properties
    bool pass = deviceObj->MemoryTypeFromProperties(memoryRequirements.memoryTypeBits,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &texture->memoryAlloc.memoryTypeIndex);

    // Allocate the memory for buffer objects
    error = vkAllocateMemory(deviceObj->device, &texture->memoryAlloc, NULL, &(texture->mem));

    // Bind the image device memory
    error = vkBindImageMemory(deviceObj->device, texture->image, texture->mem, 0);

    VkImageSubresource subresource	= {};
    subresource.aspectMask			= VK_IMAGE_ASPECT_COLOR_BIT;
    subresource.mipLevel			= 0;
    subresource.arrayLayer			= 0;

    VkSubresourceLayout layout;
    uint8_t *data;

    vkGetImageSubresourceLayout(deviceObj->device, texture->image, &subresource, &layout);

    // Map the GPU memory on to local host
    error = vkMapMemory(deviceObj->device, texture->mem, 0, texture->memoryAlloc.allocationSize, 0, (void**)&data);
    assert(!error);

    // Load image texture data in the mapped buffer
    uint8_t* dataTemp = (uint8_t*)image2D.data();
    for (int y = 0; y < texture->textureHeight; y++)
    {
        size_t imageSize = texture->textureHeight * 4;
        memcpy(data, dataTemp, imageSize);
        dataTemp += imageSize;

        // Advance row by row pitch information
        data += layout.rowPitch;
    }

    // UnMap the host memory to push the changes into the device memory
    vkUnmapMemory(deviceObj->device, texture->mem);

    // Command buffer allocation and recording begins
    CommandBufferMgr::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdTexture);
    CommandBufferMgr::beginCommandBuffer(cmdTexture);

    VkImageSubresourceRange subresourceRange	= {};
    subresourceRange.aspectMask					= VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel				= 0;
    subresourceRange.levelCount					= texture->mipMapLevels;
    subresourceRange.layerCount					= 1;

    texture->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SetImageLayout(texture->image, VK_IMAGE_ASPECT_COLOR_BIT,
                   VK_IMAGE_LAYOUT_PREINITIALIZED, texture->imageLayout,
                   subresourceRange, cmdTexture);

    // Stop command buffer recording
    CommandBufferMgr::endCommandBuffer(cmdTexture);

    // Ensure that the GPU has finished the submitted job before host takes over again
    VkFence fence;
    VkFenceCreateInfo fenceCI = {};
    fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCI.flags = 0;

    vkCreateFence(deviceObj->device, &fenceCI, nullptr, &fence);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdTexture;

    CommandBufferMgr::submitCommandBuffer(deviceObj->queue, &cmdTexture, &submitInfo, fence);
    vkWaitForFences(deviceObj->device, 1, &fence, VK_TRUE, 10000000000);
    vkDestroyFence(deviceObj->device, fence, nullptr);

    // Specify a particular kind of texture using samplers
    VkSamplerCreateInfo samplerCI	= {};
    samplerCI.sType					= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCI.pNext					= NULL;
    samplerCI.magFilter				= VK_FILTER_LINEAR;
    samplerCI.minFilter				= VK_FILTER_LINEAR;
    samplerCI.mipmapMode			= VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCI.addressModeU			= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.addressModeV			= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.addressModeW			= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.mipLodBias			= 0.0f;
    if (deviceObj->deviceFeatures.samplerAnisotropy == VK_TRUE)
    {
        samplerCI.anisotropyEnable	= VK_TRUE;
        samplerCI.maxAnisotropy		= 8;
    }
    else
    {
        samplerCI.anisotropyEnable	= VK_FALSE;
        samplerCI.maxAnisotropy		= 1;
    }
    samplerCI.compareOp				= VK_COMPARE_OP_NEVER;
    samplerCI.minLod				= 0.0f;
    samplerCI.maxLod				= 0.0f; // Set to texture->mipLevels if Optimal tiling, generally linear does not support mip-maping
    samplerCI.borderColor			= VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    samplerCI.unnormalizedCoordinates = VK_FALSE;

    // Create the sampler
    error = vkCreateSampler(deviceObj->device, &samplerCI, NULL, &texture->sampler);
    assert(!error);

    // Create image view to allow shader to access the texture information -
    VkImageViewCreateInfo viewCI	= {};
    viewCI.sType					= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewCI.pNext					= NULL;
    viewCI.viewType					= VK_IMAGE_VIEW_TYPE_2D;
    viewCI.format					= format;
    viewCI.components.r				= VK_COMPONENT_SWIZZLE_R;
    viewCI.components.g				= VK_COMPONENT_SWIZZLE_G;
    viewCI.components.b				= VK_COMPONENT_SWIZZLE_B;
    viewCI.components.a				= VK_COMPONENT_SWIZZLE_A;
    viewCI.subresourceRange			= subresourceRange;
    viewCI.subresourceRange.levelCount = 1;
    viewCI.flags					= 0;
    viewCI.image					= texture->image;

    error = vkCreateImageView(deviceObj->device, &viewCI, NULL, &texture->view);
    assert(!error);

    texture->descsImgInfo.sampler		= texture->sampler;
    texture->descsImgInfo.imageView		= texture->view;
    texture->descsImgInfo.imageLayout	= VK_IMAGE_LAYOUT_GENERAL;

    //Save texture data
    texturesData[filename] = texture;
}
void VulkanRenderer::CreateTextureOptimal(const char *filename, VkImageUsageFlags imageUsageFlags, VkFormat format) {
    CreateTextureLinear(filename, imageUsageFlags, format);
}
void VulkanRenderer::DestroyCommandBuffer() {
    VkCommandBuffer cmdBuffers[] = { cmdDepthImage, cmdVertexBuffer, cmdTexture };
    vkFreeCommandBuffers(deviceObj->device, cmdPool, sizeof(cmdBuffers) / sizeof(VkCommandBuffer), cmdBuffers);
}
void VulkanRenderer::DestroyCommandPool() {
    VulkanDevice* deviceObj = application->deviceObj;

    vkDestroyCommandPool(deviceObj->device, cmdPool, NULL);
}
void VulkanRenderer::DestroyDepthBuffer() {
    vkDestroyImageView(deviceObj->device, Depth.view, NULL);
    vkDestroyImage(deviceObj->device, Depth.image, NULL);
    vkFreeMemory(deviceObj->device, Depth.mem, NULL);
}
void VulkanRenderer::DestroyDrawableVertexBuffer() {
    for (VulkanDrawable* drawableObj : drawableList) {
        drawableObj->DestroyVertexBuffer();
    }
}
void VulkanRenderer::DestroyRenderPass() {
    vkDestroyRenderPass(deviceObj->device, renderPass, NULL);
}
void VulkanRenderer::DestroyFrameBuffers() {
    for (uint32_t i = 0; i < swapChainObj->scPublicVars.swapchainImageCount; i++) {
        vkDestroyFramebuffer(deviceObj->device, framebuffers.at(i), NULL);
    }
    framebuffers.clear();
}
void VulkanRenderer::DestroyPipeline() {
    for (VkPipeline* pipeline : pipelineList)
    {
        vkDestroyPipeline(deviceObj->device, *pipeline, NULL);
        free(pipeline);
    }
    pipelineList.clear();
}
void VulkanRenderer::DestroyDrawableCommandBuffer() {
    for (VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->DestroyCommandBuffer();
    }
}
void VulkanRenderer::DestroyDrawableSynchronizationObjects() {
    for (VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->DestroySynchronizationObjects();
    }
}
void VulkanRenderer::DestroyDrawableUniformBuffer() {
    for (VulkanDrawable* drawableObj : drawableList)
    {
        drawableObj->DestroyUniformBuffer();
    }
}
void VulkanRenderer::DestroyTextureResource() {
    for(auto iter : texturesData) {
        TextureData texture = *iter.second;
        vkFreeMemory(deviceObj->device, texture.mem, NULL);
        vkDestroySampler(deviceObj->device, texture.sampler, NULL);
        vkDestroyImage(deviceObj->device, texture.image, NULL);
        vkDestroyImageView(deviceObj->device, texture.view, NULL);
    }
}
