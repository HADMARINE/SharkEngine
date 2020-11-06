//
// Created by EunwooSong on 2020-11-06.
//

#ifndef SHARKENGINE_CAMERACOMPONENT_H
#define SHARKENGINE_CAMERACOMPONENT_H

#include "include/Engine/Scene/Components/CameraComponent.h"

namespace SharkEngine::Core {
    class CameraComponent : public Component {
    public:
        CameraComponent(){};
        ~CameraComponent(){};

        void Render() override;
    };
}


#endif//SHARKENGINE_CAMERACOMPONENT_H
