//
// Created by EunwooSong on 2020-11-06.
//

#include "StartScene.h"
#include "InGame/GameObjects/Buttons/ButtonObj.h"
#include "include/Engine/Scene/Components/Button.h"
#include "include/Engine/Scene/Components/Transform.h"
#include "include/Engine/Scene/Components/Sprite2DRenderer.h"

using namespace SharkEngine::Core;

StartScene::StartScene() {
}
StartScene::~StartScene() {
}
void StartScene::Init() {
    Scene::Init();

    ButtonObj* btn_start = new ButtonObj(); btn_start->Init();
    ButtonObj* btn_exit = new ButtonObj(); btn_exit->Init();
    //ButtonObj* btn_help = new ButtonObj(); btn_help->Init();

    btn_start->GetComponent<Button>()->SetTextureImage("source/UI/Button/GameStart.png");
    btn_start->GetComponent<Button>()->SetOnClick([=]{
        CLogger::Info("StartScene");
    });
    btn_start->GetComponent<Transform>()->SetLocalPos(
            Vec2(1280, 720) / 2.0f -
                    btn_start->GetComponent<Sprite2DRenderer>()->GetTextureSize() / 2.0f -
                    Vec2(0, 0));

    btn_exit->GetComponent<Button>()->SetTextureImage("source/UI/Button/GameStart.png");
    btn_exit->GetComponent<Button>()->SetOnClick([=]{
      CLogger::Info("StartScene");
    });
    btn_exit->GetComponent<Transform>()->SetLocalPos(
            Vec2(1280, 720) / 2.0f -
                    btn_exit->GetComponent<Sprite2DRenderer>()->GetTextureSize() / 2.0f -
                    Vec2(0, 100.0f));
}
