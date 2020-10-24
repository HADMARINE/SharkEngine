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

    class SignatureManager {
    public:
        template<typename T>
        shared_ptr<T> RegisterSystem();

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
