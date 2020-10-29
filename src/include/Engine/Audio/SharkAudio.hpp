//
// Created by HADMARINE on 2020/10/27.
//

#ifndef VULKAN_ENGINE_SHARKAUDIO_HPP
#define VULKAN_ENGINE_SHARKAUDIO_HPP

#include "../../../stdafx.hpp"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <glm/glm.hpp>



namespace SharkEngine {
    class Audio {
    private:
#define checkAudioError(_msg)                   \
    if (!GlobalPreferences::DEBUG) {            \
        error = alGetError();                   \
        if (error != AL_NO_ERROR) {             \
            CLogger::Error("[AUDIO] %s", _msg); \
            return -1;                          \
        }                                       \
    }
    public:
        struct SourcePreferences {
            glm::vec3 pos;
            glm::vec3 vel;
            float pitch;
            float gain;
            bool loop;
        };

        struct ListenerPreferences {
            glm::vec3 pos;
            glm::vec3 vel;
            glm::vec4 ori;
        };

        struct SourceStruct{
            ALuint *source;
            ALuint *buffer;
        };


        Audio();
        ~Audio();

        int setListenerPreferences(ListenerPreferences *pref) {
            ALenum error;

            this->listenerPref = pref;

            ALfloat ori[]{pref->ori.w, pref->ori.x, pref->ori.y, pref->ori.z};

            alListener3f(AL_POSITION, pref->pos.x, pref->pos.y, pref->pos.z);
            checkAudioError("Set listener position");

            alListener3f(AL_VELOCITY, pref->vel.x, pref->vel.y, pref->vel.z);
            checkAudioError("Set listener velocity");

            alListenerfv(AL_ORIENTATION, ori);
            checkAudioError("Set listener orientation");

            return 0;
        }

        ListenerPreferences* getListenerPreferences() {
            return listenerPref;
        }

        static int initialize() {
            ALboolean enumeration;
            const ALCchar *defaultDeviceName;

            ALCenum error;

            CLogger::Debug("[AUDIO] Using ALUT for backend utility");

            enumeration = alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT");
            if (enumeration == AL_FALSE) {
                CLogger::Error("[AUDIO] Enumeration extension is not available.");
            }

            listAudioDevices(alcGetString(nullptr, ALC_DEVICE_SPECIFIER));

            if (!defaultDeviceName) {
                defaultDeviceName = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
            }

            device = alcOpenDevice(defaultDeviceName);

            if (!device) {
                CLogger::Error("[AUDIO] Unable to open default device");
                return -1;
            }

            CLogger::Debug("[AUDIO] Device : %s", alcGetString(device, ALC_DEVICE_SPECIFIER));

            alGetError();

            context = alcCreateContext(device, nullptr);
            if (!alcMakeContextCurrent(context)) {
                CLogger::Error("[AUDIO] Failed to make default context");
            }

            checkAudioError("Make default context");
        }

        static void cleanup() {
            for (auto source : Audio::sources) {
                alDeleteSources(1, source.source);
                alDeleteBuffers(1, source.buffer);
            }

            device = alcGetContextsDevice(context);
            alcMakeContextCurrent(nullptr);
            alcDestroyContext(context);
            alcCloseDevice(device);
        }


    private:
        // Member Variable
        static std::vector<SourceStruct> sources;
        static ALCdevice *device;
        static ALCcontext *context;
        static ListenerPreferences *listenerPref;


        static void listAudioDevices(const ALCchar *devices) {
            const ALCchar *device = devices, *next = devices + 1;
            size_t len = 0;

            CLogger::Debug("[AUDIO] Devices List\n");

            while (device && *device != '\0' && next && *next != '\0') {
                CLogger::Debug("[AUDIO] %s", device);
                len = strlen(device);
                device += (len + 1);
                next += (len + 2);
            }
        }

        static inline ALenum toAlFormat(short channels, short samples) {
            bool stereo = (channels > 1);

            switch (samples) {
                case 16:
                    if (stereo)
                        return AL_FORMAT_STEREO16;
                    else
                        return AL_FORMAT_MONO16;
                case 8:
                    if (stereo)
                        return AL_FORMAT_STEREO8;
                    else
                        return AL_FORMAT_MONO8;
                default:
                    return -1;
            }
        }

        int loadAudio(const char *filename, const SourcePreferences& pref) {
            ALuint source, buffer;
            ALCenum error;
            ALsizei size, freq;
            ALenum format;
            ALvoid *data;
            ALboolean loop = pref.loop ? AL_TRUE : AL_FALSE;

            alGenSources((ALuint)1, &source);
            checkAudioError("Source generation");

            alSourcef(source, AL_PITCH, pref.pitch);
            checkAudioError("Source Pitch");

            alSourcef(source, AL_GAIN, pref.gain);
            checkAudioError("Source Gain");

            alSource3f(source, AL_POSITION, pref.pos.x, pref.pos.y, pref.pos.z);
            checkAudioError("Source Position");

            alSource3f(source, AL_VELOCITY, pref.vel.x, pref.vel.y, pref.vel.z);
            checkAudioError("Source Velocity");

            alSourcei(source, AL_LOOPING, loop);
            checkAudioError("Source Loop");

            alGenBuffers(1, &buffer);
            checkAudioError("Buffer generation");

            alutLoadWAVFile((ALbyte *) filename, &format, &data, &size, &freq);
            checkAudioError("Load wav file");

            alBufferData(buffer, format, data, size, freq);
            checkAudioError("Buffer copy");

            alSourcei(source, AL_BUFFER, buffer);
            checkAudioError("Buffer binding");

            SourceStruct sourceStruct{};
            sourceStruct.buffer = &buffer;
            sourceStruct.source = &source;

            sources.push_back(sourceStruct);
            return sources.size() - 1;
        }

        int play(unsigned int idx) {
            ALCenum error;
            auto src = *sources.at(idx).source;

            alSourcePlay(src);
            checkAudioError("Play");
            return 0;
        }

        int pause(unsigned int idx) {
            ALCenum error;
            auto src = *sources.at(idx).source;

            alSourcePause(src);
            checkAudioError("Pause");
            return 0;
        }

        int stop(unsigned int idx) {
            ALCenum error;
            auto src = *sources.at(idx).source;

            alSourceStop(src);
            checkAudioError("Stop");
            return 0;
        }

        int rewind(unsigned int idx) {
            ALCenum error;
            auto src = *sources.at(idx).source;

            alSourceRewind(src);
            checkAudioError("Rewind");
            return 0;
        }

        ALint getState(unsigned int idx) {
            ALCenum error;
            ALint sourceState;

            auto src = *sources.at(idx).source;

            alGetSourcei(src, AL_SOURCE_STATE, &sourceState);
            checkAudioError("Source state get");

            while(sourceState == AL_PLAYING) {
                alGetSourcei(src, AL_SOURCE_STATE, &sourceState);
                checkAudioError("Source state get");
            }

            return sourceState;
        }

        int updateSourcePrefences(unsigned int idx, SourcePreferences &pref) {
            ALCenum error;

            auto src = *sources.at(idx).source;

            alSourcef(src, AL_PITCH, pref.pitch);
            checkAudioError("Source Pitch");

            alSourcef(src, AL_GAIN, pref.gain);
            checkAudioError("Source Gain");

            alSource3f(src, AL_POSITION, pref.pos.x, pref.pos.y, pref.pos.z);
            checkAudioError("Source Position");

            alSource3f(src, AL_VELOCITY, pref.vel.x, pref.vel.y, pref.vel.z);
            checkAudioError("Source Velocity");

            alSourcei(src, AL_LOOPING, pref.loop ? AL_TRUE : AL_FALSE);
            checkAudioError("Source Loop");

            return 0;
        }


        SourcePreferences getSourcePreferences(unsigned int idx) {
            return reinterpret_cast<SourcePreferences &&>(sources.at(idx));
        }
    };

    std::vector<Audio::SourceStruct> Audio::sources{};
    ALCdevice *Audio::device = nullptr;
    ALCcontext *Audio::context = nullptr;
    Audio::ListenerPreferences *Audio::listenerPref = nullptr;


}// namespace SharkEngine

#endif//VULKAN_ENGINE_SHARKAUDIO_HPP
