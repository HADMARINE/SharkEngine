//
// Created by EunwooSong on 2020-11-05.
//

#ifndef SHARKENGINE_SPRITEANIMATION_H
#define SHARKENGINE_SPRITEANIMATION_H

#include "Base/Component.h"
#include "include/Engine/SharkSystem.hpp"

namespace SharkEngine::Core {
    class SpriteAnimation {
    public:
        SpriteAnimation() : fps(0),
                            textureCount(0),
                            isLoop(true),
                            isLoopEnd(true),
                            isEnd(true),
                            currentFrame(0)
                            {};
        ~SpriteAnimation(){};

        void Update();

        void AddTextures(std::string root, int textureCount);
        void AddTexture(const char* path);

        void ResetTexture();
        void RestartAnimation();

        int GetFps() {return fps;}
        int GetTextureCount() {return textureCount;}
        bool GetIsAnimationLoop() {return isLoop;}
        bool GetIsAnimationEnd() {return isLoopEnd || isEnd;}
        float GetCurrentFrame() {return currentFrame;}
        LPDIRECT3DTEXTURE9* GetCurrentFrameTexture() { return textures[(int)currentFrame]; }

        void SetFps(int fps) {this->fps = fps;}
        void SetIsLoop(bool isLoop) {this->isLoop = isLoop;}

    private:
        std::vector<LPDIRECT3DTEXTURE9*> textures;
        int fps;
        int textureCount;
        bool isLoop;
        bool isLoopEnd;
        bool isEnd;
        float currentFrame;
        float timeCheck;
    };
}


#endif//SHARKENGINE_SPRITEANIMATION_H
