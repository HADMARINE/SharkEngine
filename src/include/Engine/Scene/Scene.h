//
// Created by EunwooSong on 2020-10-23.
//

#ifndef VULKAN_ENGINE_SCENE_H
#define VULKAN_ENGINE_SCENE_H

#include <iostream>
#include <list>
#include <map>
#include "../CoreTypes.h"

using namespace std;

namespace SharkEngine::Core {
    class Entity;
    class EntityIDManager;
    class ComponentManager;
    class SignatureManager;

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

        virtual void Init();
        void Update();
        void LateUpdate();
        void Render();
        void EndScene();

        list<EntityID> m_EntityList;

    private:
        unique_ptr<ComponentManager> m_ComponentManager;
        unique_ptr<EntityIDManager> m_EntityManager;
        unique_ptr<SignatureManager> m_SystemManager;
    };
}

#endif//VULKAN_ENGINE_SCENE_H
