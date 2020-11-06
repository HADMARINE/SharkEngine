//
// Created by EunwooSong on 2020-11-06.
//

#ifndef SHARKENGINE_BUTTON_H
#define SHARKENGINE_BUTTON_H

#include "include/Engine/Scene/Components/Base/Component.h"
#include "include/Engine/CoreDefine.h"

namespace SharkEngine::Core {
    class Button : public Component {
    public:
        Button();
        ~Button();

        void Start() override;
        void Update() override;


        void SetTextureImage(const char* path);

        Rect GetBoundaryRect();
        void SetBoundaryRect(Rect boundaryRect);
    };
}


#endif//SHARKENGINE_BUTTON_H
