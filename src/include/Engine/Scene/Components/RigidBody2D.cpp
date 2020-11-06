//
// Created by HADMARINE on 2020-11-06.
//

#include "RigidBody2D.hpp"
#include "BoxCollider.hpp"
#include "include/Engine/SharkSystem.hpp"

namespace SharkEngine::Core {
    void SharkEngine::Core::RigidBody2D::eval() {
        auto *collider = GetOwner()->GetComponent<BoxCollider>();

        auto deltaTime = SHARK_ENGINE->GetTimeManager()->GetDeltaTime();

        Vec2 vel{0, 0};

        vel.y += deltaTime * GetGravity();

        AddVelocity(vel);

        auto *transform = GetOwner()->GetComponent<Transform>();
    }
}
