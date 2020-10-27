//
// Created by HADMARINE on 2020/10/27.
//

#ifndef VULKAN_ENGINE_SHARKAUDIO_HPP
#define VULKAN_ENGINE_SHARKAUDIO_HPP

#include <AL/al.h>
#include <AL/alc.h>
#include "../../../stdafx.hpp"

namespace SharkEngine {
#define checkAudioError(_msg)
    if(GlobalPreferences::)
    error = alGetError();
    if(error != AL_NO_ERROR)
\


    class Audio {
    public:
        Audio();
        ~Audio();
    private:
        void listAudioDevices(const ALCchar *devices) {
            const ALCchar *device = devices, *next = devices + 1;
            size_t len = 0;

            CLogger::Debug("[AUDIO] Devices List\n");

            while(device && *device != '\0' && next && *next != '\0') {
                CLogger::Debug("[AUDIO] %s", device);
                len = strlen(device);
                device += (len + 1);
                next += (len + 2);
            }

        }
    };
}

#endif//VULKAN_ENGINE_SHARKAUDIO_HPP
