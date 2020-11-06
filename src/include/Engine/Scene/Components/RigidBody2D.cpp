//
// Created by HADMARINE on 2020-11-06.
//

#include "RigidBody2D.hpp"
//#include "BoxCollider.hpp"
#include "include/Engine/SharkSystem.hpp"
#include "Transform.h"

namespace SharkEngine::Core {
    void SharkEngine::Core::RigidBody2D::LateUpdate() {
//        auto *collider = GetOwner()->GetComponent<BoxCollider>();

        auto deltaTime = SHARK_ENGINE->GetTimeManager()->GetDeltaTime();

        Vec2 vel{0, 0};

        vel.y += deltaTime * GetGravity();

        AddVelocity(vel);

        GetOwner()->transform->Translate(Vec2(deltaTime * GetVelocity().x, deltaTime * GetVelocity().y));
    }
}
