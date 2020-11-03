//
// Created by HADMARINE on 2020/11/01.
//

#pragma once

#include <glm/glm.hpp>
#include "RigidBody.hpp"

namespace SharkEngine::Core::Physics {
    struct XYMinMax {
        struct xy {
            float x;
            float y;
        };
        xy min;
        xy max;
    };

    void setMinMax(RigidBody<AABB> rigid, XYMinMax xymm) {
        xymm.max.x = rigid.object.pos.x + rigid.object.scale.x;
        xymm.max.y = rigid.object.pos.y + rigid.object.scale.y;
        xymm.min.x = rigid.object.pos.x - rigid.object.scale.x;
        xymm.min.y = rigid.object.pos.y - rigid.object.scale.y;
    }


    //Circle vs Circle

    bool EvalCollision(RigidBody<Circle> a, RigidBody<Circle> b) {
        float r = a.object.radius + b.object.radius;
        return pow(r, 2) < pow(a.object.pos.x + b.object.pos.x,
                               2) + pow(a.object.pos.y + b.object.pos.y, 2);
    }

    //AABB vs AABB

    bool EvalCollision(RigidBody<AABB> A, RigidBody<AABB> B) {

        XYMinMax a{}, b{};

        setMinMax(A, a);
        setMinMax(B, b);

        if (a.max.x < b.min.x or a.min.x > b.max.x) return false;
        if (a.max.y < b.min.y or a.min.y > b.max.y) return false;

        return true;
    }

    // OBB vs OBB


    namespace OBBFunctions {
        using namespace glm;

        vec3 getHeightVector(RigidBody<OBB> a) {
            vec3 ret;
            ret.x = a.object.scale.y * sinf(a.object.rot) / 2;
            ret.y = -a.object.scale.y * cosf(a.object.rot) / 2;
            return ret;
        }

        vec3 getWidthVector(RigidBody<OBB> a) {
            vec3 ret;
            ret.x = a.object.scale.x * cosf(a.object.rot) / 2;
            ret.y = -a.object.scale.x * sinf(a.object.rot) / 2;
            return ret;
        }

        vec3 getUnitVector(vec3 a) {
            vec3 ret;
            float size;
            size = sqrt(pow(a.x, 2) + pow(a.y, 2));
            ret.x = a.x / size;
            ret.y = a.y / size;
            return ret;
        }

#define PI 3.14159265358979323846f

        float deg_to_rad(float deg) {
            return deg * PI / 180;
        }

        float rad_to_deg(float rad) {
            return rad * 180 / PI;
        }

        float dotProduct(vec2 & l, vec2 & r) {
            return l.x * r.x + l.y * r.y;
        }

        float dotProduct(vec3 & l, vec3 & r) {
            return l.x * r.x + l.y * r.y + l.z * r.z;
        }

        vec2 crossProduct(vec2 & l, vec2 & r) {
            return l * r;
        }

    }// namespace OBBFunctions

    bool EvalCollision(RigidBody<OBB> a, RigidBody<OBB> b) {
        using namespace glm;
        using namespace OBBFunctions;
        vec3 dist = a.object.pos - b.object.pos;
        vec3 vec[4] = {
                getHeightVector(a),
                getHeightVector(b),
                getWidthVector(a),
                getWidthVector(b)};
        vec3 unit;
        for (auto &i : vec) {
            double sum = 0;
            unit = getUnitVector(i);
            for (auto &j : vec) {
                sum += abs(j.x * unit.x + j.y * unit.y);
            }
            if (abs(dist.x * unit.x + dist.y * unit.y) > sum) {
                return false;
            }
        }
        return true;
    }

    // AABB vs OBB

    bool EvalCollision(RigidBody<AABB> _a, RigidBody<OBB> b) {
        RigidBody<OBB> a{};
        a.object.pos = _a.object.pos;
        a.object.scale = _a.object.scale;
        a.object.rot = 0;
        a.vel = _a.vel;
        a.mass = _a.mass;
        a.strength = _a.strength;

        return EvalCollision(a, b);
    }

    bool EvalCollision(RigidBody<OBB> b, RigidBody<AABB> a) {
        return EvalCollision(a, b);
    }

    // Circle vs AABB

    bool EvalCollision(RigidBody<Circle> C, RigidBody<AABB> B) {
        XYMinMax b{};
        setMinMax(B, b);

        float x = fmax(b.min.x, fmin(C.object.pos.x, b.max.x));
        float y = fmax(b.min.y, fmin(C.object.pos.y, b.max.y));

        float distance = pow(x - C.object.pos.x, 2) + pow(y - C.object.pos.y, 2);

        return distance < pow(C.object.radius, 2);
    }

    bool EvalCollision(RigidBody<AABB> B, RigidBody<Circle> C) {
        return EvalCollision(C, B);
    }

    // Circle vs OBB

    bool EvalCollision(RigidBody<Circle> _a, RigidBody<OBB> b) {
        using namespace glm;
        using namespace OBBFunctions;

        RigidBody<OBB> a{};
        a.object.scale = vec3(_a.object.radius, _a.object.radius, _a.object.radius);
        a.object.pos = _a.object.pos;
        a.object.rot = 0;
        a.strength = _a.strength;
        a.mass = _a.mass;
        a.vel = _a.vel;

        return EvalCollision(a, b);
    }


    bool EvalCollision(RigidBody<OBB> b, RigidBody<Circle> c) {
        return EvalCollision(c, b);
    }

    template<typename T, typename U>
    void ResolveCollision(RigidBody<OBB> A, RigidBody<OBB> B) {
//        TODO : Tweak this function
        using namespace glm;
        using namespace OBBFunctions;

        // Calculate relative velocity
        vec3 rv = B.vel - A.vel;
        vec3 auto normal = crossProduct(A.object.pos, B.object.pos);

        // Calculate relative velocity in terms of the normal direction
        float velAlongNormal = dotProduct(rv, normal);

        // Do not resolve if velocities are separating
        if (velAlongNormal > 0)
            return;

        // Calculate restitution
        float e = min(A.restitution, B.restitution);



        // Calculate impulse scalar
        float j = -(1 + e) * velAlongNormal;
        j /= 1 / A.mass + 1 / B.mass;

        // Apply impulse
        vec2 impulse = j * normal;


        auto owner = A.GetOwner();
        owner->GetCurrentScene()->GetComponent<>(owner->GetEntityID());
        A.velocity -= 1 / A.mass * impulse;
        B.velocity += 1 / B.mass * impulse;
    }

    template<typename T, typename U>
    void ResolveCollisionPriorOnly(RigidBody<T> A, RigidBody<U> B) {
        using namespace glm;
        using namespace OBBFunctions;

        // Calculate relative velocity
        vec3 rv = B.vel - A.vel;
        auto normal = crossProduct(A.object.pos, B.object.pos);

        // Calculate relative velocity in terms of the normal direction
        float velAlongNormal = dotProduct(rv, normal);

        // Do not resolve if velocities are separating
        if (velAlongNormal > 0)
            return;

        // Calculate restitution
        float e = min(A.restitution, B.restitution);

        // Calculate impulse scalar
        float j = -(1 + e) * velAlongNormal;
        j /= 1 / A.mass + 1 / B.mass;

        // Apply impulse
        vec2 impulse = j * normal;

        A.m_Owner->
        A.velocity -= 1 / A.mass * impulse;
        B.velocity += 1 / B.mass * impulse;
    }


}// namespace SharkEngine::Core::Physics