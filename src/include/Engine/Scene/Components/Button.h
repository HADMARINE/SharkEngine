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

        Rect GetBoundaryRect() {return boundaryRect;}
        void SetBoundaryRect(Rect boundaryRect) {this->boundaryRect = boundaryRect;}

        bool GetIsOnCursor() {return isOnCursor;}
        bool GetIsOnClick() {return isOnClick;}
        bool GetIsEndClick() {return isEndClick;}

    private:
        bool isOnCursor;
        bool isOnClick;
        bool isEndClick;

        Rect boundaryRect;
    };
}


#endif//SHARKENGINE_BUTTON_H
