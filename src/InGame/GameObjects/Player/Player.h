//
// Created by EunwooSong on 2020-11-06.
//

#ifndef SHARKENGINE_PLAYER_H
#define SHARKENGINE_PLAYER_H

#include "include/Engine/Scene/GameObject/GameObject.h"
#include "InGame/Scripts/Player/PlayerMovement.h"

class Player : public SharkEngine::Core::GameObject {
public:
    Player() : GameObject(){};
    ~Player(){};

    void Init() override;

    PlayerMovement* playerMovement;
};


#endif//SHARKENGINE_PLAYER_H
