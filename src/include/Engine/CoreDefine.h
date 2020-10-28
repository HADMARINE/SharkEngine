//
// Created by EunwooSong on 2020-10-02.
//

#ifndef VULKAN_ENGINE_COREDEFINE_H
#define VULKAN_ENGINE_COREDEFINE_H

#include "glm/glm.hpp"
#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <queue>

//DELETE Macro
#define SAFE_DELETE(p)     \
    {                      \
        if (p) {           \
            delete (p);    \
            (p) = nullptr; \
        }                  \
    }

#define SAFE_DELETE_ARRAY(p) \
    {                        \
        if (p) {             \
            delete[](p);     \
            (p) = nullptr;   \
        }                    \
    }

#define SAFE_RELEASE(p)     \
    {                       \
        if (p) {            \
            (p)->Release(); \
            (p) = nullptr;  \
        }                   \
    }

#endif//VULKAN_ENGINE_COREDEFINE_H
