//
// Created by EunwooSong on 2020-11-03.
//

#ifndef SHARKENGINE_VULKANDRAWABLEMANAGER_H
#define SHARKENGINE_VULKANDRAWABLEMANAGER_H

#include <vector>

#define VULKAN_DRAW_MANAGER VulkanDrawableManager::Instance()

class VulkanDrawable;

class VulkanDrawableManager {
public:
    static VulkanDrawableManager* Instance();

    void CheckInit();
    void Render();
    void EndScene();

    void ReCreateSwapChain();
    void ReleaseAll();

    void AddVulkanDrawable(VulkanDrawable* drawIter);
    void DestroyVulkanDrawable(VulkanDrawable* drawIter);

private:
    std::vector<VulkanDrawable*> drawablesList;
};


#endif//SHARKENGINE_VULKANDRAWABLEMANAGER_H
