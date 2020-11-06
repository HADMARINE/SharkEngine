//
// Created by EunwooSong on 2020-11-06.
//

#ifndef SHARKENGINE_PLAYERMOVEMENT_H
#define SHARKENGINE_PLAYERMOVEMENT_H

#include "CLogger.hpp"
#include "InGame/Scenes/Test/TestScene.h"
#include "include/Engine/Scene/Components/Base/Component.h"
#include "include/Engine/Scene/Components/Sprite2DRenderer.h"
#include "include/Engine/Scene/Components/Transform.h"
#include "include/Engine/Scene/GameObject/GameObject.h"
#include "include/Engine/Scene/Components/SpriteAnimation.h"
#include "include/Engine/SharkSystem.hpp"

class PlayerMovement : public SharkEngine::Core::Component {
public:
    PlayerMovement(){};
    ~PlayerMovement();

    void Start() override;
    void Update() override;
    void LateUpdate() override;

public:
    float moveSpeed;

    SharkEngine::Core::SpriteAnimation* currentFrame;
    SharkEngine::Core::SpriteAnimation* Idle;
    SharkEngine::Core::SpriteAnimation* Move;
}

#endif//SHARKENGINE_PLAYERMOVEMENT_H
