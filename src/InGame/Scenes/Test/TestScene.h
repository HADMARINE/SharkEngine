//
// Created by EunwooSong on 2020-10-30.
//

#ifndef SHARKENGINE_TESTSCENE_H
#define SHARKENGINE_TESTSCENE_H

#include "include/Engine/Scene/Scene.h"
class TestScene : public SharkEngine::Core::Scene {
public:
    TestScene(){};
    ~TestScene(){};

    void Init() override;
};

#endif//SHARKENGINE_TESTSCENE_H
