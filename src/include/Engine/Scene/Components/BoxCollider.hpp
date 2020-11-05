//
// Created by HADMARINE on 2020-11-05.
//

#ifndef VULKAN_ENGINE_BOXCOLLIDER_HPP
#define VULKAN_ENGINE_BOXCOLLIDER_HPP

#include "Base/Component.h"
#include "include/Engine/CoreDefine.h"
#include "include/Engine/Scene/GameObject/GameObject.h"

namespace SharkEngine::Core {
    class BoxCollider : public Component {
    public:
        BoxCollider() = default;

        Vec2 GetLeftTopPos() {
            Transform* transform = GetOwner()->GetComponent<Transform>();
            return this->relativeLeftTopPos + transform->GetWorldPos();
        }

        Vec2 GetRightBottomPos() {
            Transform* transform = GetOwner()->GetComponent<Transform>();
            return this->relativeRightBottomPos + transform->GetWorldPos();
        }

        void SetLeftTopPos(Vec2 &v) {
            this->relativeLeftTopPos = v;
        }

        void SetRightBottomPos(Vec2 &v) {
            this->relativeRightBottomPos = v;
        }

    private:
        Vec2 relativeLeftTopPos;
        Vec2 relativeRightBottomPos;
    };
}

#endif //VULKAN_ENGINE_BOXCOLLIDER_HPP
