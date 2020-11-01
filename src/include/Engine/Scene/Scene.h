//
// Created by EunwooSong on 2020-10-23.
//

#pragma once

#include "../CoreTypes.h"
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <shared_mutex>
#include <vector>
#include "../../../stdafx.hpp"
#include "../../../include/Engine/Scene/Manager/ComponentManager.h"
#include "../../../include/Engine/Scene/Manager/EntityIDManager.h"
#include "../../../include/Engine/Scene/Manager/SignatureManager.h"

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
            return m_EntityIDManager->CreateEntityID();
        }

        void DestroyEntity(EntityID _id){
            m_EntityIDManager->DestroyEntityID(_id);
            m_ComponentManager->EntityDestroyed(_id);
            m_SignatureManager->EntityDestroyed(_id);
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
        void AddComponent(EntityID _id, Component* iter) {
            m_ComponentManager->AddComponent<T>(_id, iter);

            auto signature = m_EntityIDManager->GetSignature(_id);
            signature.set(m_ComponentManager->GetComponentID<T>(), true);
            m_EntityIDManager->SetSignature(_id, signature);
            m_SignatureManager->EntitySignatureChanged(_id, signature);
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
        std::shared_ptr<T> RegisterSystem() {
            return m_SignatureManager->RegisterScene<T>();
        }

        template <typename T>
        void SetSystemSignature(Signature signature){
            m_SignatureManager->SetSignature<T>(signature);
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
        std::unique_ptr<SignatureManager> m_SignatureManager;
    };
}

