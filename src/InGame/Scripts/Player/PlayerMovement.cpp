//
// Created by EunwooSong on 2020-11-06.
//

#include "PlayerMovement.h"
#include "include/Engine/SharkSystem.hpp"
#include "include/Engine/Scene/Components/Sprite2DRenderer.h"

using namespace SharkEngine::Core;

PlayerMovement::~PlayerMovement() {
    SAFE_DELETE(currentFrame);
    SAFE_DELETE(Idle);
    SAFE_DELETE(Move);
}
void PlayerMovement::Start() {
    Component::Start();

    Idle = new SpriteAnimation();
    Move = new SpriteAnimation();

    Idle->SetFps(4);
    Move->SetFps(6);

    Idle->AddTextures("source/InGame/PlayerAnimation/Idle", 3);
    Move->AddTextures("source/InGame/PlayerAnimation/Move", 8);
    currentFrame = Idle;

    GetOwner()->transform->SetScaleCenter(GetOwner()->GetComponent<Sprite2DRenderer>()->GetTextureSize() / 2.0f);
}
void PlayerMovement::Update() {
    Component::Update();

    currentFrame->Update();

    Vec2 MoveDir = Vec2(0,0);

    if(SHARK_INPUT_MGR->GetKeyState('A') == KEY_STAY) {
        MoveDir = Vec2(-1.0f, 0.0f);
        GetOwner()->transform->SetScale(Vec2(-1.0f, 1.0f));
        isMove = true;
    }
    else if(SHARK_INPUT_MGR->GetKeyState('D') == KEY_STAY) {
        MoveDir = Vec2(1.0f, 0.0f);
        GetOwner()->transform->SetScale(Vec2(1.0f, 1.0f));
        isMove = true;
    }
    else {
        isMove = false;
    }

    GetOwner()->transform->Translate(MoveDir * moveSpeed * SHARK_TIME_MGR->GetDeltaTime());

    if(isMove) {
        if(currentFrame != Move) {
            currentFrame = Move;
            currentFrame->RestartAnimation();
        }
    }
    else {
        if(currentFrame != Idle) {
            currentFrame = Idle;
            currentFrame->RestartAnimation();
        }
    }
}
void PlayerMovement::LateUpdate() {
    Component::LateUpdate();
}
void PlayerMovement::Render() {
    Component::Render();
    GetOwner()->GetComponent<Sprite2DRenderer>()->SetTexture(currentFrame->GetCurrentFrameTexture());
}