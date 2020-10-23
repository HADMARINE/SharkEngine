//
// Created by S  ongEunwoo on 2020/10/12.
//

#include "../../../../include/Engine/Scene/Manager/EntityIDManager.h"
using namespace SharkEngine::Core;
EntityIDManager::EntityIDManager() {
    for (EntityID entity = 0; entity < MAX_ENTITIES; ++entity)
    {
        m_AvailableEntityIDs.push(entity);
    }
}

EntityID EntityIDManager::CreateEntityID() {
    EntityID id = m_AvailableEntityIDs.front();
    m_AvailableEntityIDs.pop();
    ++m_LivingEntityIDCount;

    return id;
}

void EntityIDManager::DestroyEntityID(EntityID _id) {
    m_Signatures[_id].reset();

    m_AvailableEntityIDs.push(_id);
    --m_LivingEntityIDCount;
}

void EntityIDManager::SetSignature(EntityID _id, Signature signature) {
    m_Signatures[_id] = signature;
}

Signature EntityIDManager::GetSignature(EntityID _id) {
    return m_Signatures[_id];
}
