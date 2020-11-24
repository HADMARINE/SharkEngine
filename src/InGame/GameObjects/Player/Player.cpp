//
// Created by EunwooSong on 2020-11-06.
//

#include "Player.h"

#include "include/Engine/Scene/Components/Sprite2DRenderer.h"
#include "include/Engine/Scene/Components/RigidBody2D.hpp"
#include "include/Engine/Scene/Components/BoxCollider.hpp"

using namespace SharkEngine::Core;

void Player::Init() {
    GameObject::Init();

    AddComponent<Sprite2DRenderer>()->SetTexture("source/InGame/PlayerAnimation/Idle/1.png");
    AddComponent<RigidBody2D>();
    BoxCollider* box = AddComponent<BoxCollider>();
    box->SetLeftTopPos(Vec2(0,0));
    box->SetRightBottomPos(GetComponent<Sprite2DRenderer>()->GetTextureSize());

    playerMovement = AddComponent<PlayerMovement>();
}
