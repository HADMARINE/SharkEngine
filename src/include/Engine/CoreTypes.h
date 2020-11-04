//
// Created by SongEunwoo on 2020/10/12.
//

#ifndef VULKAN_ENGINE_CORETYPES_H
#define VULKAN_ENGINE_CORETYPES_H

#include <bitset>
#include <cstdint>

namespace SharkEngine::Core {
    using EntityID = std::uint32_t;
    constexpr const EntityID MAX_ENTITIES = 100000;
    using ComponentID = std::uint8_t;
    const ComponentID MAX_COMPONENTS = 1000;
    using Signature = std::bitset<MAX_COMPONENTS>;
}

#endif//VULKAN_ENGINE_CORETYPES_H
