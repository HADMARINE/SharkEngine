//
// Created by EunwooSong on 2020-10-23.
//

#ifndef VULKAN_ENGINE_SIGNATUREMANAGER_H
#define VULKAN_ENGINE_SIGNATUREMANAGER_H

#include <shared_mutex>
#include "../CoreTypes.h"

using namespace std;

namespace SharkSystem::Core {
    class SignatureManager {
    public:
        template<typename T>
        shared_ptr<T> RegisterSystem();

        template<typename T>
        void SetSignature(Signature signature);
    };
}


#endif//VULKAN_ENGINE_SIGNATUREMANAGER_H
