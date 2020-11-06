//
// Created by EunwooSong on 2020-11-06.
//

#ifndef SHARKENGINE_TESTUIOBJECT_H
#define SHARKENGINE_TESTUIOBJECT_H

#include "include/Engine/Scene/GameObject/GameObject.h"

class TestUIObject : public SharkEngine::Core::GameObject {
public:
    TestUIObject() : GameObject() {};
    ~TestUIObject(){};

    void Init() override;
};


#endif//SHARKENGINE_TESTUIOBJECT_H
