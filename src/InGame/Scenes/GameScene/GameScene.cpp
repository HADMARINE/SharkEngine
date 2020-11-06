//
// Created by EunwooSong on 2020-11-06.
//

#include "GameScene.h"
#include "InGame/GameObjects/Player/Player.h"
#include "InGame/Scripts/Player/PlayerMovement.h"

GameScene::GameScene() {
}
GameScene::~GameScene() {
}
void GameScene::Init() {
    Scene::Init();
    RegisterComponent<PlayerMovement>();

    Player* p = new Player(); p->Init();
    p->transform->SetLocalPos(Vec2(500, 500));
}
