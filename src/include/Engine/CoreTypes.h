//
// Created by SongEunwoo on 2020/10/12.
//

#ifndef VULKAN_ENGINE_CORETYPES_H
#define VULKAN_ENGINE_CORETYPES_H

#include <bitset>
#include <cstdint>

using EntityID = std::uint32_t;
const EntityID MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

#endif//VULKAN_ENGINE_CORETYPES_H
