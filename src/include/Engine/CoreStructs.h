//
// Created by EunwooSong on 2020-10-30.
//

#pragma once

#include "Scene/Entity/Entity.h"
#include <vector>

#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/quaternion.hpp>
//#include <glm/gtx/transform.hpp>

/*************** Collision **************/
namespace SharkEngine::Core {
    struct Collision {
        Entity *entity;
        std::vector<glm::vec3> contacts;//Contact points
    };

    namespace Physics {
        template <typename T>
        class RigidBody {
        public:
            T object;
            glm::vec3 vel;
            float mass;
            float strength;
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
    }// namespace Physics
}// namespace SharkEngine::Core


#endif//SHARKENGINE_CORESTRUCTS_H
