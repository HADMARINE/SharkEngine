//
// Created by EunwooSong on 2020-11-06.
//
#include "include/Engine/Scene/Components/Sprite2DRenderer.h"
#include "include/Engine/Scene/Components/Button.h"
#include "include/Engine/Scene/Components/Transform.h"

#include "CLogger.hpp"

#include "ButtonObj.h"

using namespace SharkEngine::Core;

void ButtonObj::Init() {
    GameObject::Init();

    AddComponent<Sprite2DRenderer>();
    Button* tmp = AddComponent<Button>();
}
