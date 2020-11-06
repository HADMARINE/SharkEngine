//
// Created by EunwooSong on 2020-11-06.
//

#include "Button.h"
#include "../GameObject/GameObject.h"
#include "Sprite2DRenderer.h"

SharkEngine::Core::Button::Button() {
}
SharkEngine::Core::Button::~Button() {
}
void SharkEngine::Core::Button::Start() {
    Component::Start();
}
void SharkEngine::Core::Button::Update() {
    Component::Update();
}
void SharkEngine::Core::Button::SetTextureImage(const char *path) {
    Sprite2DRenderer* renderer = GetOwner()->GetComponent<Sprite2DRenderer>();
    renderer->SetTexture(path);
    Vec2(renderer->GetWidth(), renderer->GetHeight());
}