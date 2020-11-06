//
// Created by joonl on 2020-11-06.
//

#ifndef VULKAN_ENGINE_FLOOROBJECT_HPP
#define VULKAN_ENGINE_FLOOROBJECT_HPP

#include "include/Engine/Scene/GameObject/GameObject.h"
#include "../Scripts/FloorComponent.hpp"

class FloorComponent;

class FloorObject : public SharkEngine::Core::GameObject {
public:
    ~FloorObject(){};

    void Init() override;

    FloorComponent* floorComponent;
    FloorObject() : GameObject() {}
};


#endif //VULKAN_ENGINE_FLOOROBJECT_HPP
