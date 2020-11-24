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
        void SetPosition(vec3 pos)  { position = vec4(pos, 1.0f); }
        vec3 GetLocalPosition()     { return localPosition; };
        void SetLocalPosition(vec3 pos)     { localPosition = vec4(pos, 1.0f); }
        vec3 GetRightDirection()    { return right; }
        vec3 GetLeftDirection()     { return right * vec3(-1.0f, -1.0f, -1.0f); }
        vec3 GetUpDirection()       { return up; }
        vec3 GetForwardDirection()  { return forward; }
        vec3 GetBackwardDirection()  { return forward * vec3(-1.0f, -1.0f, -1.0f); }

        vec3 GetEulerAngles()       { return eulerAngles; }
        vec3 GetLocalEulerAngles()  { return localEulerAngles; }
        vec3 GetLossyScale()        { return lossyScale; }
        vec3 GetLocalScale()        { return localScale; }
        mat4 GetLocalToWorldMatrix() { return localToWorldMatrix; }
        mat4 GetWorldToLocalMatrix() { return worldToLocalMatrix; }
        Transform *GetParent()      { return parent; };
        Transform *GetChild(int index = 0)  { if (0 <= index && index < children.size()) return children[index]; }

        //Controll ChildMethods
        void AddChild(Transform* iter);
        void SetParent(Transform* target, Transform* children);

        //Transform Method
        vec3 TransformDirection(vec3 position);
        vec3 TransformDirection(vec4 position);
        vec3 TransformDirection(float x, float y, float z);
        vec3 InverseTransformDirection(vec3 position);
        vec3 InverseTransformDirection(vec4 position);
        vec3 InverseTransformDirection(float x, float y, float z);
        vec3 TransformPoint(vec3 point);
        vec3 TransformPoint(float x, float y, float z);
        vec3 InverseTransformPoint(vec3 point);
        vec3 InverseTransformPoint(float x, float y, float z);
        void LookAt(Transform * transform);
        void LookAt(vec3 position);
        void Rotate(vec3 rotate);
        void Rotate(float x, float y, float z);
        void Translate(vec3);

    private:
        //Position, Direction, Angles, Quaternion, scale, matrix
        vec4 localPosition; //Local
        vec4 position;      //World

        vec3 right;
        vec3 up;
        vec3 forward;

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
