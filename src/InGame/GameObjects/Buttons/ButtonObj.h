//
// Created by EunwooSong on 2020-11-06.
//

#ifndef SHARKENGINE_BUTTONOBJ_H
#define SHARKENGINE_BUTTONOBJ_H

#include "include/Engine/Scene/GameObject/GameObject.h"

class ButtonObj : public SharkEngine::Core::GameObject {
public:
    ButtonObj() {};
    ~ButtonObj() {};

    void Init() override;
};


#endif//SHARKENGINE_BUTTONOBJ_H
