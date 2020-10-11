//
// Created by EunwooSong on 2020-10-02.
//

#ifndef VULKAN_ENGINE_COREDEFINE_H
#define VULKAN_ENGINE_COREDEFINE_H

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <array>
#include <queue>

//DELETE Macro
#define SAFE_DELETE(p)  \
	{                   \
		if (p) {        \
			delete (p); \
			(p) = 0;    \
		}               \
	}

#define SAFE_DELETE_ARRAY(p) \
	{                        \
		if (p) {             \
			delete[](p);     \
			(p) = 0;         \
		}                    \
	}

#define SAFE_RELEASE(p)     \
	{                       \
		if (p) {            \
			(p)->Release(); \
			(p) = 0;        \
		}                   \
	}

#endif//VULKAN_ENGINE_COREDEFINE_H
