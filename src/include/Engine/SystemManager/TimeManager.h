//
// Created by EunwooSong on 2020-10-31.
//

#ifndef SHARKENGINE_TIMEMANAGER_H
#define SHARKENGINE_TIMEMANAGER_H

#include "../../Graphic/VulkanApplication.h"

#include <GLFW/glfw3.h>

namespace SharkEngine::Core {
    class TimeManager {
    public:
        TimeManager() : oldTime(0), currentTime(0) {}
        ~TimeManager() {}

        void Init() {
            oldTime = glfwGetTime();
            currentTime = glfwGetTime();
        }

        void Update() {
            currentTime = glfwGetTime();
            deltaTime = currentTime - oldTime;
            oldTime = currentTime;
        }

        double GetDeltaTime() { return deltaTime; }
    private:
        GLFWwindow* window;
        double oldTime;
        double currentTime;

        double deltaTime;
    };
}


#endif//SHARKENGINE_TIMEMANAGER_H
