//
// Created by EunwooSong on 2020-10-31.
//

#pragma once

#include <Windows.h>

namespace SharkEngine::Core {
    class TimeManager {
    public:
        TimeManager() {}
        ~TimeManager() {}

        void Init() {
            QueryPerformanceCounter(&beforeInterval);
            QueryPerformanceCounter(&currentInterval);
            QueryPerformanceFrequency(&Frequency);
        }

        void Update() {
            QueryPerformanceCounter(&currentInterval);
            LONGLONG interval = (currentInterval.QuadPart - beforeInterval.QuadPart);

            float dTime = (float)interval / (float)Frequency.QuadPart;

            beforeInterval = currentInterval;
            deltaTime = dTime;
        }

        double GetDeltaTime() { return deltaTime; }

    private:
        double deltaTime;
        LARGE_INTEGER beforeInterval, currentInterval, Frequency;
    };
}
