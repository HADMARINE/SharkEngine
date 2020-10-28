//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_VULKANSHADER_H
#define SHARKENGINE_VULKANSHADER_H

#pragma once
#include "IncludeVulkan.h"

class VulkanShader
{
public:
    VulkanShader() {}
    ~VulkanShader() {}

    void BuildShaderModuleWithSPV(uint32_t *vertShaderText, size_t vertexSPVSize, uint32_t * fragShaderText, size_t fragmentSPVSize);
    void DestroyShaders();

#ifdef AUTO_COMPILE_GLSL_TO_SPV
    // Convert GLSL shader to SPIR-V shader
	bool GLSLtoSPV(const VkShaderStageFlagBits shaderType, const char *pshader, std::vector<unsigned int> &spirv);

	// Entry point to build the shaders
	void buildShader(const char *vertShaderText, const char *fragShaderText);

	// Type of shader language. This could be - EShLangVertex,Tessellation Control,
	// Tessellation Evaluation, Geometry, Fragment and Compute
	EShLanguage getLanguage(const VkShaderStageFlagBits shader_type);

	// Initialize the TBuitInResource
	void initializeResources(TBuiltInResource &Resources);
#endif
    VkPipelineShaderStageCreateInfo shaderStages[2];
};


#endif//SHARKENGINE_VULKANSHADER_H
