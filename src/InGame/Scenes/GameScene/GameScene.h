//
// Created by EunwooSong on 2020-11-06.
//

#ifndef SHARKENGINE_GAMESCENE_H
#define SHARKENGINE_GAMESCENE_H


#include "include/Engine/Scene/Scene.h"

class GameScene : public SharkEngine::Core::Scene {
public:
    GameScene();
    ~GameScene();

    void Init() override;
};



#endif//SHARKENGINE_GAMESCENE_H
