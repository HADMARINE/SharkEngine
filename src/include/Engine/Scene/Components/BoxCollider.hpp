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

        Vec2 GetLeftTopPos();

        Vec2 GetRightBottomPos();

        float GetRotation() const {
            return this->rotation;
        }

        bool GetIsCollided() const {
            return this->isCollided;
        }

        bool GetIsTrigger() const {
            return this->isTrigger;
        }

        void SetLeftTopPos(Vec2 v) {
            this->relativeLeftTopPos = v;
        }

        void SetRightBottomPos(Vec2 v) {
            this->relativeRightBottomPos = v;
        }

        void SetRotation(float r) {
            this->rotation = r;
        }

        void SetIsCollided(bool b) {
            this->isCollided = b;
        }

        void SetIsTrigger(bool b) {
            this->isTrigger = b;
        }

//        void Update() override;

    private:
        Vec2 relativeLeftTopPos{};
        Vec2 relativeRightBottomPos{};
        float rotation = 0.0f;
        bool isCollided = false;
        bool isTrigger = false;
    };
}

#endif //VULKAN_ENGINE_BOXCOLLIDER_HPP
