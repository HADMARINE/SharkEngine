//
// Created by EunwooSong on 2020-11-01.
//

#include "TestComponent.h"
void TestComponent::Update(){
    Component::Update();
    CLogger::Debug("%d Test Update", (int)m_Owner->GetEntityID());

//    GetOwner()->transform->Translate(Vec2(20.0f, 20.0f) * SHARK_TIME_MGR->GetDeltaTime());

    SHARK_LINE_MGR->DrawBox(GetOwner()->GetComponent<SharkEngine::Core::Transform>()->GetWorldPos(),
                            GetOwner()->GetComponent<SharkEngine::Core::Transform>()->GetWorldPos() + GetOwner()->GetComponent<SharkEngine::Core::Sprite2DRenderer>()->GetTextureSize(),
                            D3DCOLOR_ARGB(255,255,0,0));
    //GetOwner()->transform->GetMatrix());

    if(SHARK_INPUT_MGR->GetKeyState(' ') == KEY_DOWN)
        SHARK_SCENE_MGR->ChangeScene(new TestScene());
}
