//
// Created by EunwooSong on 2020-11-06.
//

#include "PlayerMovement.h"

using namespace SharkEngine::Core;

PlayerMovement::~PlayerMovement() {
    SAFE_DELETE(currentFrame);
    SAFE_DELETE(Idle);
    SAFE_DELETE(Move);
}
void PlayerMovement::Start() {
    Component::Start();

    currentFrame = new SpriteAnimation();
    Idle = new SpriteAnimation();
    Move = new SpriteAnimation();
}
void PlayerMovement::Update() {
    Component::Update();
}
void PlayerMovement::LateUpdate() {
    Component::LateUpdate();
}

