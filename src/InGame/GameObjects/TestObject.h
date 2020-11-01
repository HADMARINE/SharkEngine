//
// Created by EunwooSong on 2020-11-01.
//

#ifndef SHARKENGINE_TESTOBJECT_H
#define SHARKENGINE_TESTOBJECT_H

#include "../../include/Engine/Scene/Entity/Entity.h"

class TestComponent;

class TestObject : public SharkEngine::Core::Entity {
public:
    TestObject() : Entity() {};
    ~TestObject(){};

    void Init() override;

    TestComponent* testComponent;
};


#endif//SHARKENGINE_TESTOBJECT_H
