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
            Transform *transform = GetOwner()->GetComponent<Transform>();
            return this->relativeLeftTopPos + transform->GetWorldPos();
        }

        Vec2 GetRightBottomPos() {
            Transform *transform = GetOwner()->GetComponent<Transform>();
            return this->relativeRightBottomPos + transform->GetWorldPos();
        }

        float GetRotation() {
            return this->rotation;
        }

        bool GetIsCollided() {
            return this->isCollided;
        }

        void SetLeftTopPos(Vec2 &v) {
            this->relativeLeftTopPos = v;
        }

        void SetRightBottomPos(Vec2 &v) {
            this->relativeRightBottomPos = v;
        }

        void SetRotation(float r) {
            this->rotation = r;
        }

        void SetIsCollided(bool b) {
            this->isCollided = b;
        }

    private:
        Vec2 relativeLeftTopPos{};
        Vec2 relativeRightBottomPos{};
        float rotation = 0.0f;
        bool isCollided = false;
    };
}

#endif //VULKAN_ENGINE_BOXCOLLIDER_HPP
