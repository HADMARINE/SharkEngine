//
// Created by HADMARINE on 2020/11/01.
//

#ifndef SHARKENGINE_PHYSICS_HPP
#define SHARKENGINE_PHYSICS_HPP

#include "../../include/Engine/CoreStructs.h"
#include <glm/glm.hpp>

namespace SharkEngine::Core::Physics {
    struct XYMinmax {
        struct xy {
            float x;
            float y;
        };
        xy min;
        xy max;
    };

    void setMinMax(RigidBody<AABB> rigid, XYMinmax xymm) {
        xymm.max.x = rigid.object.pos.x + rigid.object.scale.x;
        xymm.max.y = rigid.object.pos.y + rigid.object.scale.y;
        xymm.min.x = rigid.object.pos.x - rigid.object.scale.x;
        xymm.min.y = rigid.object.pos.y - rigid.object.scale.y;

    }


    bool EvalCollision(RigidBody<Circle> a, RigidBody<Circle> b) {
        float r = a.object.radius + b.object.radius;
        r *= r;
        return r < pow(a.object.pos.x + b.object.pos.x,
                       2) + pow(a.object.pos.y + b.object.pos.y, 2);
    }

    bool EvalCollision(RigidBody<AABB> A, RigidBody<AABB> B) {

        XYMinmax a{}, b{};

        setMinMax(A, a);
        setMinMax(B, b);

        if (a.max.x < b.min.x or a.min.x > b.max.x) return false;
        if (a.max.y < b.min.y or a.min.y > b.max.y) return false;

        return true;
    }

    bool EvalCollision(RigidBody<OBB> A, RigidBody<OBB> B) {

    }



    void ResolveCollision(RigidBody<Box> A, RigidBody<Box> B) {
        // Calculate relative velocity
        glm::vec3 rv = B.vel - A.vel;

        // Calculate relative velocity in terms of the normal direction
        float velAlongNormal = DotProduct(rv, normal);

        // Do not resolve if velocities are separating
        if (velAlongNormal > 0)
            return;

        // Calculate restitution
        float e = min(A.restitution, B.restitution);

        // Calculate impulse scalar
        float j = -(1 + e) * velAlongNormal;
        j /= 1 / A.mass + 1 / B.mass;

        // Apply impulse
        glm::vec2 impulse = j * normal;
        A.velocity -= 1 / A.mass * impulse;
        B.velocity += 1 / B.mass * impulse;
    }



    namespace OBBFunctions {
        using namespace glm;
        vec3 getHeightVector(Box a) {
            vec3 ret;
            ret.x = a.
        }
    }

    bool OBB(Box A, Box B) {

    }

}// namespace SharkEngine::Core::Physics::Dim2

#endif//SHARKENGINE_PHYSICS_HPP
