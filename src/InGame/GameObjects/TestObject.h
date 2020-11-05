//
// Created by EunwooSong on 2020-11-01.
//

#ifndef SHARKENGINE_TESTOBJECT_H
#define SHARKENGINE_TESTOBJECT_H

#include "include/Engine/Scene/GameObject/GameObject.h"

class TestComponent;

class TestObject : public SharkEngine::Core::GameObject {
public:
    TestObject() : GameObject() {};
    ~TestObject(){};

    void Init() override;

    TestComponent* testComponent;
};


#endif//SHARKENGINE_TESTOBJECT_H
