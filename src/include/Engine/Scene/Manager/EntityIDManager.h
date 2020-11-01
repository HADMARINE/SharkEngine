//
// Created by SongEunwoo on 2020/10/12.
//

#ifndef VULKAN_ENGINE_ENTITYIDMANAGER_H
#define VULKAN_ENGINE_ENTITYIDMANAGER_H

#include "../../CoreTypes.h"
#include <array>
#include <cassert>
#include <queue>

namespace SharkEngine::Core {
    class EntityIDManager {
    public:
        EntityIDManager();
        ~EntityIDManager() {};

        //When you create some game object, get the EntityID from the current Scenes EntityIDManager.
        EntityID CreateEntityID() {
            assert(m_LivingEntityIDCount < MAX_ENTITIES && "Entity out of range.");

            EntityID id = m_AvailableEntityIDs.front();
            m_AvailableEntityIDs.pop();
            ++m_LivingEntityIDCount;

            return id;
        }
        // When some game objects are destroyed, return the EntityID and add the AvailableEntityID.
        void DestroyEntityID(EntityID _id) {
            assert(_id < MAX_ENTITIES && "Entity out of range.");
            m_Signatures[_id].reset();

            m_AvailableEntityIDs.push(_id);
            --m_LivingEntityIDCount;
        }


        void SetSignature(EntityID _id, Signature signature){
            assert(_id < MAX_ENTITIES && "Entity out of range.");

            m_Signatures[_id] = signature;
        }

        Signature GetSignature(EntityID _id) {
            assert(_id < MAX_ENTITIES && "Entity out of range.");

            return m_Signatures[_id];
        }

    private:
        // Queue of unu`sed entity IDs
        std::queue<EntityID> m_AvailableEntityIDs{};
        // Signature array granted to the entity ID
        std::array<Signature, MAX_ENTITIES> m_Signatures{};
        // The current number of entities in the scene
        uint32_t m_LivingEntityIDCount{};
    };
}

#endif//VULKAN_ENGINE_ENTITYIDMANAGER_H
