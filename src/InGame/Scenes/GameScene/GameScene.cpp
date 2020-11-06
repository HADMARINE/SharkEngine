//
// Created by EunwooSong on 2020-11-06.
//

#include "GameScene.h"
#include "InGame/GameObjects/Player/Player.h"
#include "InGame/Scripts/Player/PlayerMovement.h"
#include "include/Engine/Scene/Components/RigidBody2D.hpp"
#include "include/Engine/Scene/Components/BoxCollider.hpp"

using namespace SharkEngine::Core;

GameScene::GameScene() {
}
GameScene::~GameScene() {
}
void GameScene::Init() {
    Scene::Init();
    RegisterComponent<PlayerMovement>();

    Player* p = new Player(); p->Init();
    p->transform->SetLocalPos(Vec2(500, 300));

    GameObject* floor = new GameObject(); floor->Init();
    floor->AddComponent<Sprite2DRenderer>()->SetTexture("source/InGame/Floor/floor2.png");
    floor->AddComponent<RigidBody2D>()->SetGravity(0.0f);
    BoxCollider* tmp = floor->AddComponent<BoxCollider>();
    floor->transform->SetLocalPos(Vec2(300, 600));
    tmp->SetLeftTopPos(Vec2(300, 600));
    tmp->SetRightBottomPos(Vec2(300, 600) + floor->GetComponent<Sprite2DRenderer>()->GetTextureSize());

    GameObject* ui = new GameObject(); ui->Init();
    ui->AddComponent<Sprite2DRenderer>()->SetTexture("source/UI/asdf.png");
    ui->transform->SetScale(Vec2(0.65f, 0.65f));
}
