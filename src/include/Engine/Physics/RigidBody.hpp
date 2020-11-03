//
// Created by HADMARINE on 2020/11/02.
//

#ifndef SHARKENGINE_RIGIDBODY_HPP
#define SHARKENGINE_RIGIDBODY_HPP


#include "../Scene/Components/Base/Component.h"
#include <functional>
#include <glm/glm.hpp>
#include "../SharkSystem.hpp"

namespace SharkEngine::Core::Physics {
    template<typename T>
    class RigidBody : public Component {
    public:
        T object;
        glm::vec3 vel;
        float mass;
        float strength;

        bool isGravity = true;
        bool isTrigger = false;
        bool isCollided = false;
        glm::vec2 normal;

        std::function<void()> onCollisionEnter;
        std::function<void()> onCollisionStay;
        std::function<void()> onCollisionExit;

    public:
        RigidBody() = default;
        RigidBody(T object, glm::vec3 vel, float mass,
                  float strength, bool isGravity, bool isTrigger,
                  const std::function<void()> &onCollisionEnter,
                  const std::function<void()> &onCollisionStay,
                  const std::function<void()> &onCollisionExit) {
            this->object = object;
            this->vel = vel;
            this->mass = mass;
            this->strength = strength;
            this->isGravity = isGravity;
            this->isTrigger = isTrigger;
//            this->normal = new glm::vec2();
            this->onCollisionEnter = onCollisionEnter;
            this->onCollisionStay = onCollisionStay;
            this->onCollisionExit = onCollisionExit;
        }

        template<typename U, typename V>
        static bool Assert(RigidBody<U> a, RigidBody<V> b) {
            const auto result = EvalCollision(a, b);

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
                    a.onCollisionEnter();
                } else {
                    a.onCollisionStay();
                }

                if (!b.isCollided) {
                    b.onCollisionEnter();
                } else {
                    b.onCollisionStay();
                }

                a->isCollided = true;
                b->isCollided = true;
            } else {
                if (a.isCollided) {
                    a.onCollisionExit();
                }

                if (b.isCollided) {
                    b.onCollisionExit();
                }

                a->isCollided = false;
                b->isCollided = false;
            }
        }
    };

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

#endif//SHARKENGINE_RIGIDBODY_HPP
