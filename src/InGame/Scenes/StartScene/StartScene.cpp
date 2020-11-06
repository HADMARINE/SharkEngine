//
// Created by EunwooSong on 2020-11-06.
//

#include "StartScene.h"
#include "InGame/GameObjects/Buttons/ButtonObj.h"
#include "include/Engine/Scene/Components/Button.h"
#include "include/Engine/Scene/Components/Transform.h"
#include "include/Engine/Scene/Components/Sprite2DRenderer.h"
#include "include/Engine/Scene/Components/CameraComponent.h"
#include "include/Engine/SharkSystem.hpp"
#include "InGame/Scenes/Test/TestScene.h"
using namespace SharkEngine::Core;

StartScene::StartScene() {
}
StartScene::~StartScene() {
}
void StartScene::Init() {
    Scene::Init();

    GameObject* cam = new GameObject(); cam->Init();
    cam->AddComponent<CameraComponent>();
    //cam->GetComponent<Transform>()->SetScale(Vec2(0.75f, 0.75f));

    GameObject* bg = new GameObject(); bg->Init();
    bg->AddComponent<Sprite2DRenderer>()->SetTexture("source/UI/Background/Main.png");
    bg->GetComponent<Transform>()->SetScale(Vec2(0.831f, 0.831f));

    GameObject* logo = new GameObject(); logo->Init();
    logo->AddComponent<Sprite2DRenderer>()->SetTexture("source/UI/Logo/Logo.png");
    logo->GetComponent<Transform>()->SetScale(Vec2(0.831f, 0.831f));
    logo->GetComponent<Transform>()->SetLocalPos(Vec2(1280, 0) / 2.0f -
                                                 logo->GetComponent<Sprite2DRenderer>()->GetTextureSize() / 2.0f +
                                                 Vec2(0, 200.0));
    logo->GetComponent<Transform>()->SetScaleCenter(logo->GetComponent<Sprite2DRenderer>()->GetTextureSize() / 2.0f);

    ButtonObj* btn_start = new ButtonObj(); btn_start->Init();
    ButtonObj* btn_exit = new ButtonObj(); btn_exit->Init();
    ButtonObj* btn_option = new ButtonObj(); btn_option->Init();
    //ButtonObj* btn_help = new ButtonObj(); btn_help->Init();

    btn_start->GetComponent<Button>()->SetTextureImage("source/UI/Button/GameStart.png");
    btn_start->GetComponent<Button>()->SetOnClick([=]{
        CLogger::Info("StartScene");
        SHARK_SCENE_MGR->ChangeScene(new TestScene());
    });
    btn_start->GetComponent<Transform>()->SetLocalPos(
            Vec2(1280, 720) / 2.0f -
                    btn_start->GetComponent<Sprite2DRenderer>()->GetTextureSize() / 2.0f +
                    Vec2(0, 50));
    btn_start->GetComponent<Button>()->SetDefScale(Vec2(0.831f, 0.831f));
    btn_start->GetComponent<Button>()->SetUpScale(Vec2(0.9f, 0.9f));

    btn_exit->GetComponent<Button>()->SetTextureImage("source/UI/Button/Exit.png");
    btn_exit->GetComponent<Button>()->SetOnClick([=]{
      CLogger::Info("Game Exit!");
      exit(0);
    });
    btn_exit->GetComponent<Transform>()->SetLocalPos(
            Vec2(1280, 720) / 2.0f -
                    btn_exit->GetComponent<Sprite2DRenderer>()->GetTextureSize() / 2.0f +
                    Vec2(0, 175));
    btn_exit->GetComponent<Button>()->SetDefScale(Vec2(0.831f, 0.831f));
    btn_exit->GetComponent<Button>()->SetUpScale(Vec2(0.9f, 0.9f));

    btn_option->GetComponent<Button>()->SetTextureImage("source/UI/Button/Option.png");
    btn_option->GetComponent<Button>()->SetOnClick([=]{
        btn_option->SetIsDestroy(true);
        CLogger::Info("Test Destroy!");
    });
    btn_option->GetComponent<Transform>()->SetLocalPos(
            Vec2(1150, 625) -
                    btn_option->GetComponent<Sprite2DRenderer>()->GetTextureSize() / 2.0f);
    btn_option->GetComponent<Button>()->SetDefScale(Vec2(0.831f, 0.831f));
    btn_option->GetComponent<Button>()->SetUpScale(Vec2(1.0f, 1.0f));
}