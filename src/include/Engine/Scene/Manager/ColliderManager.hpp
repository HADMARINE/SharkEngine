//
// Created by HADMARINE on 2020-11-06.
//

#ifndef VULKAN_ENGINE_COLLIDERMANAGER_HPP
#define VULKAN_ENGINE_COLLIDERMANAGER_HPP

#include "stdafx.hpp"

#include <unordered_set>


namespace SharkEngine::Core {
    class BoxCollider;

    class ColliderManager {
    public:
        void MountCollider(BoxCollider *colA, BoxCollider *colB);

        std::vector<std::unordered_set<BoxCollider *>> GetColliderQueue();
        void Update();
        void LateUpdate();

    private:
        std::vector<std::unordered_set<BoxCollider *>> colliderQueue{};
    };
};


#endif //VULKAN_ENGINE_COLLIDERMANAGER_HPP
