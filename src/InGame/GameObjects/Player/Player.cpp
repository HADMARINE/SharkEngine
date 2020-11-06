//
// Created by EunwooSong on 2020-11-06.
//

#include "Player.h"

#include "include/Engine/Scene/Components/Sprite2DRenderer.h"

using namespace SharkEngine::Core;

void Player::Init() {
    GameObject::Init();

    AddComponent<Sprite2DRenderer>()->SetTexture("source/InGame/PlayerAnimation/Idle/1.png");
    playerMovement = AddComponent<PlayerMovement>();
}
