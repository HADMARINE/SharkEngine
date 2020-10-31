//
// Created by EunwooSong on 2020-10-30.
//

#ifndef SHARKENGINE_ENGINESTRUCTS_H
#define SHARKENGINE_ENGINESTRUCTS_H

#include "../../stdafx.hpp"
#include "CoreDefine.h"
#include "Scene/Entity/Entity.h"

using namespace SharkEngine::Core;

/*************** Collision **************/
template<typename T>
struct Collision {
    Entity* entity;
    vector<glm::vec3> contacts;   //Contact points
};



#endif//SHARKENGINE_ENGINESTRUCTS_H
