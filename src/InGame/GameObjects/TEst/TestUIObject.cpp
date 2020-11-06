//
// Created by EunwooSong on 2020-11-06.
//

#include "TestUIObject.h"
#include "include/Engine/Scene/Components/Sprite2DRenderer.h"
#include "include/Engine/Scene/Components/Button.h"
#include "include/Engine/Scene/Components/Transform.h"

using namespace SharkEngine::Core;

void TestUIObject::Init() {
    GameObject::Init();

    AddComponent<Sprite2DRenderer>();
    AddComponent<Button>()->SetTextureImage("source/GameStart.png");

    transform->SetLocalPos(Vec2(1280, 720) / 2.0f);

}
