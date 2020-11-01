//
// Created by EunwooSong on 2020-10-30.
//

#pragma once

#include <vector>

#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/quaternion.hpp>
//#include <glm/gtx/transform.hpp>

/*************** Collision **************/
namespace SharkEngine::Core{
    class Entity;

    struct Collision {
        Entity *entity;
        std::vector<glm::vec3> contacts;   //Contact points
    };
}