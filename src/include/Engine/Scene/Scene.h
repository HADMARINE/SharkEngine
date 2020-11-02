//
// Created by EunwooSong on 2020-10-23.
//

#pragma once

#include "Manager/ComponentManager.h"
#include "Manager/EntityIDManager.h"
#include "../CoreTypes.h"
#include <set>
#include <vector>

namespace SharkEngine::Core {
    //--------------------------------------------------------------------------------------
    // Scene
    // - 컴포넌트와 서명된 엔터티의 목록을 저장하고 반복 실행을 위함
    // - 각 관리자(EntityID, Component, Signature)의 포인터를 통해 관리자끼리의 소통을 진행함
    //
    // Set 사용 이점 (-> List보다 빠르다..!)
    // - 각 언터티는 고유한 서명을 가지고 모든 요소가 고유 한 것으로 정의, 집합을 통해서도 잘 맵핑됨
    // - 삽입 / 제거가 쉬운 정수 -> 이진트리를 통해 제거 진행
    //      -> 집합 이용의 이유
    //
    // 사용 예제
    // - Entity gameObject = SHARK_SCENE_MGR->getCurrentScene()
    //--------------------------------------------------------------------------------------

    class Component;

    class Scene {
    public:
        Scene();
        ~Scene();

        //Hierarchy, Initialize gameObject
        //Ex, Player* player = new Player();
        virtual void Init() = 0;
        void Start();
        void Update();
        void LateUpdate();
        void Render();
        void EndScene();

        EntityID CreateEntity(Entity* iter){
            m_EntityList.push_back(iter);
            iter->Init();
            return m_EntityIDManager->CreateEntityID();
        }

        void DestroyEntity(EntityID _id){
            m_EntityIDManager->DestroyEntityID(_id);
            m_ComponentManager->EntityDestroyed(_id);
        }

        template <typename T>
        void RegisterComponent(){
            m_ComponentManager->RegisterComponent<T>();
        }

        template <typename T>
        void DestroyComponent(EntityID _id){
            m_ComponentManager->DestroyComponent<T>(_id);

            auto signature = m_EntityIDManager->GetSignature(_id);
            signature.set(m_ComponentManager->GetComponentID<T>(), false);
        }

        template <typename T>
        void AddComponent(Entity* _id, Component* iter) {
            m_ComponentManager->AddComponent<T>(_id->GetEntityID(), iter);
            iter->SetOwner(_id);
            auto signature = m_EntityIDManager->GetSignature(_id->GetEntityID());
            signature.set(m_ComponentManager->GetComponentID<T>(), true);
            m_EntityIDManager->SetSignature(_id->GetEntityID(), signature);
        };

        template <typename T>
        T* GetComponent(EntityID _id) {
            return m_ComponentManager->GetComponent<T>(_id);
        }

        template <typename T>
        ComponentID GetComponentType(){
            return m_ComponentManager->GetComponentID<T>();
        }

        template <typename T>
        std::array<Component*, MAX_COMPONENTS>* GetComponentArray(){
            return m_ComponentManager->GetComponentArray<T>().GetComponentArray();
        }

        std::set<EntityID> m_EntityIDList;
        std::vector<Entity*> m_EntityList;

    private:
        std::unique_ptr<ComponentManager> m_ComponentManager;
        std::unique_ptr<EntityIDManager> m_EntityIDManager;
    };
}

