//
// Created by EunwooSong on 2020-11-01.
//

#ifndef SHARKENGINE_TRANSFORM_H
#define SHARKENGINE_TRANSFORM_H

#include "../../../../stdafx.hpp"
#include "../../CoreDefine.h"
#include "../../CoreTypes.h"
#include "Base/Component.h"
#include <d3dx9.h>

namespace SharkEngine::Core {
    class Transform : public SharkEngine::Core::Component {
    public:
        Transform() :
                localPos(0, 0),
                worldPos(0, 0),
                scaleCenter(0, 0),
                scale(1, 1),
                rotationCenter(0, 0),
                rotation(0),
                parent(nullptr) {}

        ~Transform() {};

        void Start() override;

        void Update() override;

        void Render() override;

        void LateUpdate() override;

        void EndScene() override;

        void UpdateTransformation();

        Vec2 GetLocalPos() { return localPos; }

        Vec2 GetWorldPos() { return worldPos; }

        float GetRotation() { return rotation; }

        Vec2 GetScale() { return scale; }

        Transform *GetChild(int index) { return children[index]; }

        Transform *GetParent() { return parent; }

        const D3DXMATRIX& GetMatrix() { return mat; }

        void SetParent(Transform *iter) { parent = iter; };

        void SetLocalPos(Vec2 localPos) { this->localPos = localPos; };

        void SetRotation(float z) { this->rotation = z; }

        void SetRotationCenter(Vec2 rc) { rotationCenter = rc; }

        void SetScale(Vec2 scale) { this->scale = scale; }

        void SetScaleCenter(Vec2 sc) { scaleCenter = sc; }

        void Translate(Vec2 pos) { localPos += pos; }

        void AddChild(Transform *iter);

        void PopChild(Transform *iter);

    private:
        //Position, Direction, Angles, Quaternion, scale, matrix
        Vec2 localPos;
        Vec2 worldPos;      //ReadOnly
        Vec2 scaleCenter;
        Vec2 scale;
        Vec2 rotationCenter;
        float rotation;
        D3DXMATRIX mat;

        Transform *parent;
        std::vector<Transform *> children;

        //Get Methods
        //        vec3 GetPosition()          { return position; };
        //        void SetPosition(vec3 pos)  { position = vec4(pos, 1.0f); }
        //        vec3 GetLocalPosition()     { return localPosition; };
        //        void SetLocalPosition(vec3 pos)     { localPosition = vec4(pos, 1.0f); }
        //        vec3 GetRightDirection()    { return right; }
        //        vec3 GetLeftDirection()     { return right * vec3(-1.0f, -1.0f, -1.0f); }
        //        vec3 GetUpDirection()       { return up; }
        //        vec3 GetForwardDirection()  { return forward; }
        //        vec3 GetBackwardDirection()  { return forward * vec3(-1.0f, -1.0f, -1.0f); }
        //
        //        vec3 GetEulerAngles()       { return eulerAngles; }
        //        vec3 GetLocalEulerAngles()  { return localEulerAngles; }
        //        vec3 GetLossyScale()        { return lossyScale; }
        //        vec3 GetLocalScale()        { return localScale; }
        //        mat4 GetLocalToWorldMatrix() { return localToWorldMatrix; }
        //        mat4 GetWorldToLocalMatrix() { return worldToLocalMatrix; }
        //        Transform *GetParent()      { return parent; };
        //        Transform *GetChild(int index = 0)  { if (0 <= index && index < children.size()) return children[index]; }
        //
        //        //Controll ChildMethods
        //        void AddChild(Transform* iter);
        //        void SetParent(Transform* target, Transform* children);
        //
        //        //Transform Method
        //        vec3 TransformDirection(vec3 position);
        //        vec3 TransformDirection(vec4 position);
        //        vec3 TransformDirection(float x, float y, float z);
        //        vec3 InverseTransformDirection(vec3 position);
        //        vec3 InverseTransformDirection(vec4 position);
        //        vec3 InverseTransformDirection(float x, float y, float z);
        //        vec3 TransformPoint(vec3 point);
        //        vec3 TransformPoint(float x, float y, float z);
        //        vec3 InverseTransformPoint(vec3 point);
        //        vec3 InverseTransformPoint(float x, float y, float z);
        //        void LookAt(Transform * transform);
        //        void LookAt(vec3 position);
        //        void Rotate(vec3 rotate);
        //        void Rotate(float x, float y, float z);
        //        void Translate(vec3);
    };
}

#endif//SHARKENGINE_TRANSFORM_H
