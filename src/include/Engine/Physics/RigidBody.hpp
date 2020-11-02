//
// Created by HADMARINE on 2020/11/02.
//

#ifndef SHARKENGINE_RIGIDBODY_HPP
#define SHARKENGINE_RIGIDBODY_HPP


#include "../Scene/Components/Base/Component.h"
#include <glm/glm.hpp>

namespace SharkEngine::Core::Physics {
    template<typename T>
    class RigidBody : Component {
    public:
        T object;
        glm::vec3 vel;
        float mass;
        float strength;

        bool isGravity = true;
        bool isTrigger = false;

    public:
        RigidBody() = default;
        RigidBody(T object, glm::vec3 vel, float mass, float strength, bool isGravity, bool isTrigger) {
            this->object = object;
            this->vel = vel;
            this->mass = mass;
            this->strength = strength;
            this->isGravity = isGravity;
            this->isTrigger = isTrigger;
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
        glm::vec3 pos;
        float rot;
        glm::vec3 scale;
    };
}// namespace SharkEngine::Core::Physics

#endif//SHARKENGINE_RIGIDBODY_HPP
