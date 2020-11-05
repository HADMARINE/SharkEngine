//
// Created by HADMARINE on 2020/11/01.
//

#pragma once

#include <glm/glm.hpp>
#include "RigidBody2D.hpp"

namespace SharkEngine::Core::Physics {
    struct XYMinMax {
        struct xy {
            float x;
            float y;
        };
        xy min;
        xy max;
    };

    template<class CTA>
    void setMinMax(RigidBody2D<AABB, CTA> rigid, XYMinMax& xymm) {
        xymm.max.x = rigid.object.pos.x + rigid.object.scale.x;
        xymm.max.y = rigid.object.pos.y + rigid.object.scale.y;
        xymm.min.x = rigid.object.pos.x - rigid.object.scale.x;
        xymm.min.y = rigid.object.pos.y - rigid.object.scale.y;
    }


    //Circle vs Circle

    template<class CTA, class CTB>
    bool EvalCollision(RigidBody2D<Circle, CTA> a, RigidBody2D<Circle, CTB> b) {
        float r = a.object.radius + b.object.radius;
        return pow(r, 2) < pow(a.object.pos.x + b.object.pos.x,
                               2) + pow(a.object.pos.y + b.object.pos.y, 2);
    }

    //AABB vs AABB

    template<class CTA, class CTB>
    bool EvalCollision(RigidBody2D<AABB, CTA> A, RigidBody2D<AABB, CTB> B) {

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

        template<class CTA>
        vec3 getHeightVector(RigidBody2D<OBB, CTA> a) {
            vec3 ret;
            ret.x = a.object.scale.y * sinf(a.object.rot) / 2;
            ret.y = -a.object.scale.y * cosf(a.object.rot) / 2;
            return ret;
        }

        template<class CTA>
        vec3 getWidthVector(RigidBody2D<OBB, CTA> a) {
            vec3 ret;
            ret.x = a.object.scale.x * cosf(a.object.rot) / 2;
            ret.y = -a.object.scale.x * sinf(a.object.rot) / 2;
            return ret;
        }

        vec2 getUnitVector(vec2 a) {
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

    template<class CTA, class CTB>
    bool EvalCollision(RigidBody2D<OBB, CTA> a, RigidBody2D<OBB, CTB> b) {
        using namespace glm;
        using namespace OBBFunctions;
        vec2 dist = a.object.pos - b.object.pos;
        vec2 vec[4] = {
                getHeightVector(a),
                getHeightVector(b),
                getWidthVector(a),
                getWidthVector(b)};
        vec2 unit;
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

    template<class CTA, class CTB>
    bool EvalCollision(RigidBody2D<AABB, CTA> _a, RigidBody2D<OBB, CTB> b) {
        RigidBody2D<OBB, CTA> a{};
        a.object.pos = _a.object.pos;
        a.object.scale = _a.object.scale;
        a.object.rot = 0;
        a.vel = _a.vel;
        a.mass = _a.mass;
        a.restitution = _a.restitution;

        return EvalCollision(a, b);
    }

    template<class CTA, class CTB>
    bool EvalCollision(RigidBody2D<OBB, CTA> b, RigidBody2D<AABB, CTB> a) {
        return EvalCollision(a, b);
    }

    // Circle vs AABB

    template<class CTA, class CTB>
    bool EvalCollision(RigidBody2D<Circle, CTA> C, RigidBody2D<AABB, CTB> B) {
        XYMinMax b{};
        setMinMax(B, b);

        float x = fmax(b.min.x, fmin(C.object.pos.x, b.max.x));
        float y = fmax(b.min.y, fmin(C.object.pos.y, b.max.y));

        float distance = pow(x - C.object.pos.x, 2) + pow(y - C.object.pos.y, 2);

        return distance < pow(C.object.radius, 2);
    }

    template<class CTA, class CTB>
    bool EvalCollision(RigidBody2D<AABB, CTA> B, RigidBody2D<Circle, CTB> C) {
        return EvalCollision(C, B);
    }

    // Circle vs OBB

    template<class CTA, class CTB>
    bool EvalCollision(RigidBody2D<Circle, CTA> _a, RigidBody2D<OBB, CTB> b) {
        using namespace glm;
        using namespace OBBFunctions;

        RigidBody2D<OBB, CTA> a{};
        a.object.scale = vec3(_a.object.radius, _a.object.radius, _a.object.radius);
        a.object.pos = _a.object.pos;
        a.object.rot = 0;
        a.restitution = _a.restitution;
        a.mass = _a.mass;
        a.vel = _a.vel;

        return EvalCollision(a, b);
    }


    template<class CTA, class CTB>
    bool EvalCollision(RigidBody2D<OBB, CTA> b, RigidBody2D<Circle, CTA> c) {
        return EvalCollision(c, b);
    }

    template<typename _A, class CTA, typename _B, class CTB>
    void ResolveCollision(RigidBody2D<_A, CTA> A, RigidBody2D<_B, CTB> B) {
//        TODO : Tweak this function
        using namespace glm;
        using namespace OBBFunctions;

        // Calculate relative velocity
        vec2 rv = B.vel - A.vel;
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

        Entity* ownerA = A.GetOwner();
        Entity* ownerB = B.GetOwner();
        Component aComp = ownerA->GetCurrentScene()->template GetComponent<CTA>(ownerA->GetEntityID());
        Component bComp = ownerB->GetCurrentScene()->template GetComponent<CTB>(ownerB->GetEntityID());
    }

    template<typename _A, class CTA, typename _B, class CTB>
    void ResolveCollisionPriorOnly(RigidBody2D<_A, CTA> A, RigidBody2D<_B, CTB> B) {
//        using namespace glm;
//        using namespace OBBFunctions;
//
//        // Calculate relative velocity
//        vec3 rv = B.vel - A.vel;
//        auto normal = crossProduct(A.object.pos, B.object.pos);
//
//        // Calculate relative velocity in terms of the normal direction
//        float velAlongNormal = dotProduct(rv, normal);
//
//        // Do not resolve if velocities are separating
//        if (velAlongNormal > 0)
//            return;
//
//        // Calculate restitution
//        float e = min(A.restitution, B.restitution);
//
//        // Calculate impulse scalar
//        float j = -(1 + e) * velAlongNormal;
//        j /= 1 / A.mass + 1 / B.mass;
//
//        // Apply impulse
//        vec2 impulse = j * normal;
//
//        A.m_Owner->
//        A.velocity -= 1 / A.mass * impulse;
//        B.velocity += 1 / B.mass * impulse;
    }


}// namespace SharkEngine::Core::Physics