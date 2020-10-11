//
// Created by SongEunwoo on 2020/10/12.
//

#ifndef VULKAN_ENGINE_ENTITYMANAGER_H
#define VULKAN_ENGINE_ENTITYMANAGER_H

#include "CoreTypes.h"
#include <array.h>
#include <queue.h>

using namespace std;

namespace SharkEngine::Core {
    class EntityIDManager {
        EntityIDManager();
        ~EntityIDManager();

        EntityID CreateEntityID();
    };
}

#endif//VULKAN_ENGINE_ENTITYMANAGER_H
