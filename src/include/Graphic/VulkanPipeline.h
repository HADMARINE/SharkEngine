//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_VULKANPIPELINE_H
#define SHARKENGINE_VULKANPIPELINE_H

#pragma once
#include "IncludeVulkan.h"

#define NUMBER_OF_VIEWPORTS 1
#define NUMBER_OF_SCISSORS NUMBER_OF_VIEWPORTS

class VulkanShader;
class VulkanDrawable;
class VulkanDevice;
class VulkanApplication;

class VulkanPipeline {
public:
    VulkanPipeline();

    ~VulkanPipeline();

    void CreatePipelineCache();
    bool CreatePipeline(VulkanDrawable* drawableObj, VkPipeline* pipeline, VulkanShader* shaderObj, VkBool32 includeDepth, VkBool32 includeVi = true);
    void DestroyPipelineCache();

public:
    VkPipelineCache						pipelineCache;
    VulkanApplication*					appObj;
    VulkanDevice*						deviceObj;
};


#endif//SHARKENGINE_VULKANPIPELINE_H
