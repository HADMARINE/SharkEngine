//
// Created by SongEunwoo on 2020/10/12.
//

#ifndef VULKAN_ENGINE_ENTITYIDMANAGER_H
#define VULKAN_ENGINE_ENTITYIDMANAGER_H

#include "../../CoreDefine.h"
#include "../../CoreTypes.h"

using namespace std;

namespace SharkEngine::Core {
    class EntityIDManager {
    public:
        EntityIDManager();
        ~EntityIDManager() {};

        //When you create some game object, get the EntityID from the current Scenes EntityIDManager.
        EntityID CreateEntityID();
        // When some game objects are destroyed, return the EntityID and add the AvailableEntityID.
        void DestroyEntityID(EntityID);

        void SetSignature(EntityID, Signature);
        Signature GetSignature(EntityID);

    private:
        // Queue of unused entity IDs
        std::queue<EntityID> m_AvailableEntityIDs{};
        // Signature array granted to the entity ID
        std::array<Signature, MAX_ENTITIES> m_Signatures{};
        // The current number of entities in the scene
        uint32_t m_LivingEntityIDCount{};
    };
}

#endif//VULKAN_ENGINE_ENTITYIDMANAGER_H
