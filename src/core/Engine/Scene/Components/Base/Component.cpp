//
// Created by EunwooSong on 2020-10-25.
//

#include "include/Engine/Scene/Components/Base/Component.h"
#include "include/Engine/Scene/GameObject/GameObject.h"

SharkEngine::Core::GameObject *SharkEngine::Core::Component::GetOwner() {
    return dynamic_cast<GameObject *>(m_Owner);
}
