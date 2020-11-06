//
// Created by EunwooSong on 2020-11-06.
//

#ifndef SHARKENGINE_STARTSCENE_H
#define SHARKENGINE_STARTSCENE_H

#include "include/Engine/Scene/Scene.h"

class StartScene : public SharkEngine::Core::Scene {
public:
    StartScene();
    ~StartScene();

    void Init() override;
};


#endif//SHARKENGINE_STARTSCENE_H
