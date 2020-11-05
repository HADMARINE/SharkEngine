//
// Created by EunwooSong on 2020-11-05.
//

#include "include/Engine/Scene/Components/SpriteAnimation.h"
#include <string>

void SharkEngine::Core::SpriteAnimation::Start() {
    Component::Start();


}
void SharkEngine::Core::SpriteAnimation::Update() {
    Component::Update();

    currentFrame += SHARK_TIME_MGR->GetDeltaTime() * fps;

    if(isLoopEnd)
        isLoopEnd = false;

    if((int)currentFrame >= textureCount) {
        if(isLoop) {
            isLoopEnd = true;
            currentFrame = 0;
        }
        else {
            currentFrame = textureCount - 1;
            isEnd = true;
        }
    }
}
void SharkEngine::Core::SpriteAnimation::AddTextures(std::string root, int textureCount) {
    currentFrame = textureCount;
    for(int i = 0; i < textureCount; i++) {
        textures.push_back(SHARK_TEXTURE_MGR->LoadTextureFormFile((root + "/" + std::to_string(i) + ".png").c_str()));
    }
}
void SharkEngine::Core::SpriteAnimation::AddTexture(const char *path) {
    textures.push_back(SHARK_TEXTURE_MGR->LoadTextureFormFile(path));
}
void SharkEngine::Core::SpriteAnimation::ResetTexture() {
    textures.resize(0);
}
void SharkEngine::Core::SpriteAnimation::RestartAnimation() {
    currentFrame = 0;
    isEnd = false;
    isLoopEnd = false;
}