//
// Created by SongEunwoo on 2020/10/12.
//

#ifndef VULKAN_ENGINE_COMPONENTARRAY_HPP
#define VULKAN_ENGINE_COMPONENTARRAY_HPP

#include "CoreTypes.h"
#include "CoreDefine.h"
#include "Scene/Components/Component.h"
#include <unordered_map>

namespace SharkEngine::Core {
    class IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(EntityID _id) = 0;
    };

    template <typename T>
    class ComponentArray : public IComponentArray
    {
    public:
        void AddComponent (EntityID _id, T* component)
        {
            // Put new entry at end and update the maps
            size_t newIndex = m_Size;
            m_EntityToIndexMap[_id] = newIndex;
            m_IndexToEntityMap[newIndex] = _id;
            m_ComponentArray[newIndex] = component;
            m_Size++;
        }

        void DestroyComponent (EntityID _id)
        {
            size_t indexOfRemovedEntity = m_EntityToIndexMap[_id];
            size_t indexOfLastElement = m_Size - 1;
            SAFE_DELETE(
                    m_ComponentArray[indexOfRemovedEntity]);
            m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

            EntityID entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
            m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

            m_EntityToIndexMap.erase(_id);
            m_IndexToEntityMap.erase(indexOfLastElement);

            --m_Size;
        }

        T* GetComponent(EntityID _id = -1)
        {
            if(_id != -1) {
                return dynamic_cast<T>(m_ComponentArray[m_EntityToIndexMap[_id]]);
            }

            for(auto iter : m_ComponentArray) {
                T* p_tmp = dynamic_cast<T*>(iter);

                if(p_tmp != nullptr) {
                    return p_tmp;
                }
            }

            return nullptr;
        }

        void EntityDestroyed(EntityID _id) override
        {
            if (m_EntityToIndexMap.find(_id) != m_EntityToIndexMap.end())
            {
                // Remove the entity's component if it existed
                DestroyComponent(_id);
            }
        }

        std::array<Component*, MAX_ENTITIES>* GetComponentArray() {
            std::array<Component*, MAX_ENTITIES> cmp_array;
            int count = 0;

            for(int i = 0; i < MAX_ENTITYS; i++) {
                Component * tmp = dynamic_cast<Component*>(m_ComponentArray[i]);
                if(tmp != nullptr)
                    cmp_array[count++] = tmp;
            }
        }

    private:
        std::array<T*, MAX_ENTITIES> m_ComponentArray;
        std::unordered_map<EntityID, size_t> m_EntityToIndexMap;
        std::unordered_map<size_t, EntityID> m_IndexToEntityMap;

        // Total size of valid entries in the array.
        size_t m_Size;
    };
}

#endif//VULKAN_ENGINE_COMPONENTARRAY_HPP
