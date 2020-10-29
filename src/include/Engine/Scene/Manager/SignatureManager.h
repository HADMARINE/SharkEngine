//
// Created by EunwooSong on 2020-10-23.
//

#ifndef VULKAN_ENGINE_SIGNATUREMANAGER_H
#define VULKAN_ENGINE_SIGNATUREMANAGER_H

#include "../../CoreTypes.h"
#include <shared_mutex>
#include <unordered_map>

using namespace std;

namespace SharkEngine::Core {
    class Scene;

    //-----------------------------------------------------
    // Signature Manager
    // - 등록 된 씬과 그 서명의 기록을 유지, 담당
    // - 동일한 키(Typename)가 시스템 포인터 맵에 적용
    // - 엔터티의 서명이 변경되면 씬의 엔터티 목록을 업데이트 진행
    //      -> 추가 or 제거
    //-----------------------------------------------------

    class SignatureManager {
    public:
        template<typename T>
        shared_ptr<T> RegisterScene();

        template<typename T>
        void SetSignature(Signature signature);

        void EntityDestroyed(EntityID _ID);
        void EntitySignatureChanged(EntityID _ID, Signature _Signature);

    private:
        unordered_map<const char*, Signature> m_Signatures{};

        //Scene의 서명을 관리
        unordered_map<const char*, shared_ptr<Scene>> m_Scenes{};

    };
}


#endif//VULKAN_ENGINE_SIGNATUREMANAGER_H
