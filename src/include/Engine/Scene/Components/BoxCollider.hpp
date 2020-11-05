//
// Created by joonl on 2020-11-05.
//

#ifndef VULKAN_ENGINE_BOXCOLLIDER_HPP
#define VULKAN_ENGINE_BOXCOLLIDER_HPP

#include "Base/Component.h"
#include "include/Engine/Scene/GameObject/GameObject.h"

namespace SharkEngine::Core {
    class BoxCollider : public Component {
    public:
        BoxCollider() = default;
        void test() {
            auto transform = GetOwner()->GetComponent<Transform>();
        }
    };
}

#endif //VULKAN_ENGINE_BOXCOLLIDER_HPP
