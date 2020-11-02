//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_VULKANDESCRIPTOR_H
#define SHARKENGINE_VULKANDESCRIPTOR_H

#pragma once
#include "IncludeVulkan.h"

class VulkanDevice;
class VulkanApplication;

class VulkanDescriptor
{
public:
    VulkanDescriptor();
    ~VulkanDescriptor();

    // Creates descriptor pool and allocate descriptor set from it
    void CreateDescriptor(bool useTexture = true);
    void DestroyDescriptor();

    // Defines the scriptor sets layout binding and create descriptor layout
    virtual void CreateDescriptorSetLayout(bool useTexture = true) = 0;
    void DestroyDescriptorLayout();

    virtual void CreateDescriptorPool(bool useTexture = true) = 0;
    void DestroyDescriptorPool();

    virtual void CreateDescriptorResources() = 0;

    virtual void CreateDescriptorSet(bool useTexture = true) = 0;
    void DestroyDescriptorSet();

    virtual void CreatePipelineLayout() = 0;
    void DestroyPipelineLayouts();

public:
    // Pipeline layout object
    VkPipelineLayout pipelineLayout;

    std::vector<VkDescriptorSetLayout> descLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSet;

    VulkanDevice* deviceObj;
};

#endif//SHARKENGINE_VULKANDESCRIPTOR_H
