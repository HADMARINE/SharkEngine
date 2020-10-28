//
// Created by EunwooSong on 2020-10-27.
//

#ifndef SHARKENGINE_INCLUDEVULKAN_H
#define SHARKENGINE_INCLUDEVULKAN_H

#pragma once
#include "../../stdafx.hpp"

/*********** C/C++ HEADER FILES ***********/
/*********** COMPILER SPECIFIC PREPROCESSORS ***********/
#ifdef _WIN32
#pragma comment(linker, "/subsystem:console")
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define APP_NAME_STR_LEN 80
#define _CRT_SECURE_NO_WARNINGS

#else  // _WIN32

#define VK_USE_PLATFORM_XCB_KHR
#include <unistd.h>
#endif // _WIN32
/*********** GLM HEADER FILES ***********/
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*********** GLI HEADER FILES ***********/
#include <gli/gli.hpp>
#include <gli/texture2d.hpp>

/*********** VULKAN HEADER FILES ***********/
#include <vulkan/vulkan.h>
#ifdef AUTO_COMPILE_GLSL_TO_SPV
#include "SPIRV/GlslangToSpv.h"
#endif

#endif//SHARKENGINE_INCLUDEVULKAN_H
