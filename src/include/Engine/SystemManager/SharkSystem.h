//
// Created by EunwooSong on 2020-10-23.
//

#ifndef VULKAN_ENGINE_SHARKSYSTEM_H
#define VULKAN_ENGINE_SHARKSYSTEM_H

#define SHARK_ENGINE SharkSystem::Initialize()

namespace SharkEngine::Core {
    class SharkSystem {
    public:
        static SharkSystem* Initialize() {
            static SharkSystem* sys = new SharkSystem();
            return sys;
        };


    private:

    };
}


#endif//VULKAN_ENGINE_SHARKSYSTEM_H
