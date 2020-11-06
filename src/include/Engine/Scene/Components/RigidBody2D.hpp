//
// Created by HADMARINE on 2020-11-06.
//

#ifndef VULKAN_ENGINE_RIGIDBODY2D_HPP
#define VULKAN_ENGINE_RIGIDBODY2D_HPP

#include <include/Engine/CoreDefine.h>
#include "Base/Component.h"
#include "include/Engine/Scene/GameObject/GameObject.h"


namespace SharkEngine::Core {
    class RigidBody2D : public Component {
    public:
        RigidBody2D() = default;

        void LateUpdate() override;

        Vec2 GetVelocity() {
            return this->velocity;
        }

        float GetMass() {
            return this->mass;
        }

        float GetRestitution() {
            return this->restitution;
        }

        float GetGravity() {
            return this->gravity;
        }

        void SetVelocity(Vec2& v) {
            this->velocity = v;
        }

        void AddVelocity(Vec2& v) {
            this->velocity += v;
        }

        void SetMass(float m) {
            this->mass = m;
        }

        void SetRestitution(float r) {
            this->restitution = r;
        }

        void SetGravity(float g) {
            this->gravity = g;
        }

    private:
        Vec2 velocity;
        float mass;
        float restitution;
        float gravity;
    };
};


#endif //VULKAN_ENGINE_RIGIDBODY2D_HPP
