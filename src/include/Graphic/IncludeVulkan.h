//
// Created by EunwooSong on 2020-11-03.
//

#ifndef SHARKENGINE_INCLUDEVULKAN_H
#define SHARKENGINE_INCLUDEVULKAN_H

/*********** COMPILER SPECIFIC PREPROCESSORS ***********/
#if defined(_WIN32) || defined(_WIN64)
//#define WIN32_LEAN_AND_MEAN
//#define NOMINMAX
//#define VK_USE_PLATFORM_XCB_KHR
#elif defined(__APPLE__)
#define VK_USE_PLATFORM_MACOS_MVK
#endif

/*********** GLM HEADER FILES ***********/
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*********** GLI HEADER FILES ***********/
#include <gli/gli.hpp>
#include <gli/texture2d.hpp>

/*********** VULKAN HEADER FILES ***********/
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#ifdef AUTO_COMPILE_GLSL_TO_SPV
//#include "SPIRV/GlslangToSpv.h"
#endif

#endif//SHARKENGINE_INCLUDEVULKAN_H
