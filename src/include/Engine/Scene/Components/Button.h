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
        Button() :
                   isOnCursor(false),
                   isOnClick(false),
                   isEndCursor(false),
                   isEnableAnimation(true),
                   boundaryRect(0,0,0,0),
                   upScale(1.1f, 1.1f),
                   defScale(1.0f, 1.0f),
                   upSpeed(5.0f),
                   onClick([=]{})
        {}
        ~Button();

        void Start() override;
        void Update() override;

        void SetTextureImage(const char* path);

        Rect GetBoundaryRect() {return boundaryRect;}
        void SetBoundaryRect(Rect boundaryRect) {this->boundaryRect = boundaryRect;}

        bool GetIsOnCursor()        {return isOnCursor;}
        bool GetIsOnClick()         {return isOnClick;}
        bool GetIsEndCursor()        {return isEndCursor;}
        bool GetIsEnableAnimation() {return isEnableAnimation;}
        Vec2 GetUpScale()       {return upScale;}
        Vec2 GetDefScale()       {return defScale;}
        float GetUpSpeed()      {return upSpeed;}

        void SetIsEnableAnimation(bool enableAnimation) {isEnableAnimation = enableAnimation;}
        void SetUpScale(Vec2 scale) {upScale = scale;}
        void SetDefScale(Vec2 scale) {defScale = scale;}
        void SetUpSpeed(float speed) {upSpeed = speed;}

        void SetOnClick(std::function<void()> onClick) {this->onClick = std::move(onClick);}

    private:
        bool isOnCursor;
        bool isOnClick;
        bool isEndCursor;

        bool isEnableAnimation;

        Rect boundaryRect;

        Vec2 upScale;
        Vec2 defScale;
        float upSpeed;

        std::function<void()> onClick;
    };
}


#endif//SHARKENGINE_BUTTON_H
