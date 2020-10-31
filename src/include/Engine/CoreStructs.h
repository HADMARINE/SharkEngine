//
// Created by EunwooSong on 2020-10-30.
//

#ifndef SHARKENGINE_CORESTRUCTS_H
#define SHARKENGINE_CORESTRUCTS_H

#include <vector>
#include "Scene/Entity/Entity.h"

#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/quaternion.hpp>
//#include <glm/gtx/transform.hpp>

using namespace SharkEngine::Core;

/*************** Collision **************/
namespace {
    struct Collision {
        Entity* entity;
        std::vector<glm::vec3> contacts;   //Contact points
    };
}


#endif//SHARKENGINE_CORESTRUCTS_H
