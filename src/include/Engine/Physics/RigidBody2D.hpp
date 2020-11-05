//
// Created by HADMARINE on 2020/11/02.
//

#ifndef SHARKENGINE_RIGIDBODY2D_HPP
#define SHARKENGINE_RIGIDBODY2D_HPP


#include "../Scene/Components/Base/Component.h"
#include <functional>
#include <glm/glm.hpp>
#include "../SharkSystem.hpp"

namespace SharkEngine::Core::Physics {
    template<typename T, class CompType>
    class RigidBody2D : public Component {
    public:
        T object;
        glm::vec2 vel;
        float mass;
        float restitution;
        float gravity = 9.8;

        bool isTrigger = false;

    protected:
        bool isCollided = false;

    public:
        RigidBody2D() = default;
        RigidBody2D(T object, glm::vec2 vel, float mass,
                    float restitution, bool gravity, bool isTrigger) {
            this->object = object;
            this->vel = vel;
            this->mass = mass;
            this->isTrigger = isTrigger;
            this->restitution = restitution;
            this->gravity = gravity;
        }

        template<typename U, class CTA, typename V, class CTB>
        static bool Assert(RigidBody2D<U, CTA> a, RigidBody2D<V, CTB> b) {
            const auto result = EvalCollision(a, b);

            Entity* ownerA = a.GetOwner();
            Entity* ownerB = b.GetOwner();
            Component aComp = ownerA->GetCurrentScene()->template GetComponent<CTA>(ownerA->GetEntityID());
            Component bComp = ownerB->GetCurrentScene()->template GetComponent<CTB>(ownerB->GetEntityID());

            Collision aCol;
            Collision bCol;

            if (!a.isTrigger) {
                if (!b.isTrigger) {
                    ResolveCollision(a, b);
                } else {
                    ResolveCollisionPriorOnly(a, b);
                }
            } else if (!b.isTrigger) {
                ResolveCollisionPriorOnly(b, a);
            }

            if (result) {
                if (!a.isCollided) {
                    aComp.OnCollisionEnter(&aCol);
                } else {
                    aComp.OnCollisionStay(&aCol);
                }

                if (!b.isCollided) {
                    bComp.OnCollisionEnter(&bCol);
                } else {
                    bComp.OnCollisionStay(&bCol);
                }

                a->isCollided = true;
                b->isCollided = true;
            } else {
                if (a.isCollided) {
                    aComp.OnCollisionExit(&aCol);
                }

                if (b.isCollided) {
                    bComp.OnCollisionExit(&bCol);
                }

                a->isCollided = false;
                b->isCollided = false;
            }
        }
    };

    template <typename T, class RTA>
    bool loop(RigidBody2D<T, RTA> a) {
        float deltaTime;
    }

    struct Circle {
        float radius;
        glm::vec3 pos;
    };

    struct AABB {
        glm::vec3 pos;
        glm::vec3 scale;
    };

    struct OBB {
        glm::vec2 pos;
        float rot;
        glm::vec3 scale;
    };
}// namespace SharkEngine::Core::Physics

#endif//SHARKENGINE_RIGIDBODY2D_HPP
