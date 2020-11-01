//
// Created by SongEunwoo on 2020/10/12.
//

#include "../../../../include/Engine/Scene/Manager/EntityIDManager.h"

using namespace SharkEngine::Core;
EntityIDManager::EntityIDManager() {
    for (EntityID entity = 0; entity < MAX_ENTITIES; ++entity)
    {
        m_AvailableEntityIDs.push(entity);
    }
}