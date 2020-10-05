//
// Created by EunwooSong on 2020-10-03.
//

#ifndef VULKAN_ENGINE_TRANSFROM_H
#define VULKAN_ENGINE_TRANSFROM_H

#include <string>
#include "Component.h"
#include "CoreDefine.h"

using namespace glm;

namespace SharkEngine::Core::Components {

    class Transform : public Component {
    public:
        Transform(std::string _ID) : Component(_ID) {}
        Transform(std::string _ID, vec3 pos, quat rotation) : Component(_ID), localPostion(pos), position(pos), rotation(rotation) {}
        ~Transform();

        void Awake() override;
        void Update() override;
        void Render() override;
        void LateUpdate() override;
        void EndScene() override;

        void OnMessage(Message*) override;
        void SendMessage() override;

        //Get Methods
        vec3 GetPosition(){return position};
        vec3 GetLocalPosition(){return localPostion};

        vec3 GetRightDirection(){return right};
        vec3 GetUpDirection(){return up;};
        vec3 GetForwardDirection(){return forward;};
        vec3 GetEulerAngles(){return eulerAngles;};
        vec3 GetLocalEulerAngles(){return localEulerAngles;};

        quat GetRotation(){return rotation};
        quat GetLocalRotation(){return localRotation;};

        vec3 GetLossyScale(){return lossyScale;};
        vec3 GetLocalScale(){return localScale;};

        mat4 GetLocalToWorldMatrix(){return localToWorldMatrix;};
        mat4 GetWorldToLocalMatrix(){return worldToLocalMatrix;};

        vec3 TransformDirection(vec3);
        vec3 TransformDirection(float, float, float);
        vec3 InverseTransformDirection(vec3);
        vec3 InverseTransformDirection(float, float, float);
        vec3 InverseTrnasformPoint(vec3);
        vec3 InverseTrnasformPoint(float, float, float);
        void LookAt(Transform*);
        void LookAt(vec3);
        void Rotate(vec3);
        void Rotate(float, float, float);
        void RotateAround(vec3, vec3, float);
        void RotateAround(vec3, float);
        void RotateAroundLocal(vec3, float);
        void Translate(vec3);

    private:
        //Position, Direction, Angles, Quaternion, scale, matrix
        vec3 localPostion;
        vec3 position;

        vec3 right;
        vec3 up;
        vec3 forward;

        vec3 eulerAngles;
        vec3 localEulerAngles;

        quat rotation;
        quat localRotation;

        vec3 lossyScale;
        vec3 localScale;

        mat4 localToWorldMatrix;
        mat4 worldToLocalMatrix;
    };
}


#endif//VULKAN_ENGINE_TRANSFROM_H
