//
// Created by EunwooSong on 2020-11-01.
//

#ifndef SHARKENGINE_TRANSFORM_H
#define SHARKENGINE_TRANSFORM_H

#include "../../../../stdafx.hpp"
#include "../../CoreDefine.h"
#include "../../CoreTypes.h"
#include "Base/Component.h"

using namespace glm;

namespace SharkEngine::Core {
    class Transform : public SharkEngine::Core::Component {
        Transform() {}
        Transform(vec4 pos, quat rotation) : localPosition(pos), position(pos) {}
        ~Transform(){};

        void Start() override;
        void Update() override;
        void Render() override;
        void LateUpdate() override;
        void EndScene() override;

        //Get Methods
        vec3 GetPosition()          { return position; };
        vec3 GetLocalPosition()     { return localPosition; };
        vec3 GetRightDirection()    { return right; }
        vec3 GetUpDirection()       { return up; }
        vec3 GetForwardDirection()  { return forward; }
        vec3 GetEulerAngles()       { return eulerAngles; }
        vec3 GetLocalEulerAngles()  { return localEulerAngles; }
        vec3 GetLossyScale()        { return lossyScale; }
        vec3 GetLocalScale()        { return localScale; }
        mat4 GetLocalToWorldMatrix() { return localToWorldMatrix; }
        mat4 GetWorldToLocalMatrix() { return worldToLocalMatrix; }
        Transform *GetParent()      { return parent; };
        Transform *GetChild(int index = 0)  { if (0 <= index && index < children.size()) return children[index]; }

        vec3 TransformDirection(vec3);
        vec3 TransformDirection(vec4);
        vec3 TransformDirection(float, float, float);
        vec3 InverseTransformDirection(vec3);
        vec3 InverseTransformDirection(vec4);
        vec3 InverseTransformDirection(float, float, float);
        vec3 TransformPoint(vec3);
        vec3 TransformPoint(float, float, float);
        vec3 InverseTransformPoint(vec3);
        vec3 InverseTransformPoint(float, float, float);
        void LookAt(Transform *);
        void LookAt(vec3);
        void Rotate(vec3);
        void Rotate(float, float, float);
        void RotateAround(vec3, vec3, float);
        void RotateAround(vec3, float);
        void RotateAroundLocal(vec3, float);
        void Translate(vec3);

    private:
        //Position, Direction, Angles, Quaternion, scale, matrix
        vec4 localPosition; //Local
        vec4 position;      //World

        vec4 right;
        vec4 up;
        vec4 forward;

        vec3 localEulerAngles;
        vec3 eulerAngles;

        vec4 lossyScale;
        vec4 localScale;

        mat4 m_Matrix;
        mat4 localToWorldMatrix;
        mat4 worldToLocalMatrix;

        Transform *parent;
        std::vector<Transform *> children;
    };

}

#endif//SHARKENGINE_TRANSFORM_H
