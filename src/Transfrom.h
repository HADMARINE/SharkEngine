////
//// Created by EunwooSong on 2020-10-03.
////
//
//#ifndef VULKAN_ENGINE_TRANSFROM_H
//#define VULKAN_ENGINE_TRANSFROM_H
//
//#include "Component.h"
//#include "include/Engine/CoreDefine.h"
//#include <string>
//
//using namespace glm;
//
//namespace SharkEngine::Core::Components {
//
//    class Transform : public Component {
//    public:
//        Transform(std::string _ID) : Component(_ID) {}
//        Transform(std::string _ID, vec3 pos, quat rotation) : Component(_ID), localPosition(pos), position(pos), rotation(rotation) {}
//        ~Transform() {};
//
//        void Awake() override;
//        void Update() override;
//        void Render() override;
//        void LateUpdate() override;
//        void EndScene() override;
//
//        void OnMessage(Message*);
//        void SendMessage() override;
//
//        //Get Methods
//        vec3 GetPosition(){return position; };
//        vec3 GetLocalPosition(){return localPosition; };
//
//        vec3 GetRightDirection(){return right; };
//        vec3 GetUpDirection(){return up;};
//        vec3 GetForwardDirection(){return forward;};
//        vec3 GetEulerAngles(){return eulerAngles;};
//        vec3 GetLocalEulerAngles(){return localEulerAngles;};
//
//        quat GetRotation(){return rotation; };
//        quat GetLocalRotation(){return localRotation;};
//
//        vec3 GetLossyScale(){return lossyScale;};
//        vec3 GetLocalScale(){return localScale;};
//
//        mat4 GetLocalToWorldMatrix(){return localToWorldMatrix;};
//        mat4 GetWorldToLocalMatrix(){return worldToLocalMatrix;};
//
//        vec3 TransformDirection(vec3);
//        vec3 TransformDirection(vec4);
//        vec3 TransformDirection(float, float, float);
//        vec3 InverseTransformDirection(vec3);
//        vec3 InverseTransformDirection(vec4);
//        vec3 InverseTransformDirection(float, float, float);
//        vec3 TransformPoint(vec3);
//        vec3 TransformPoint(float, float, float);
//        vec3 InverseTransformPoint(vec3);
//        vec3 InverseTransformPoint(float, float, float);
//        void LookAt(Transform*);
//        void LookAt(vec3);
//        void Rotate(vec3);
//        void Rotate(float, float, float);
//        void RotateAround(vec3, vec3, float);
//        void RotateAround(vec3, float);
//        void RotateAroundLocal(vec3, float);
//        void Translate(vec3);
//
//    private:
//        //Position, Direction, Angles, Quaternion, scale, matrix
//        vec4 localPosition; //Local
//        vec4 position;      //World
//
//        vec4 right;
//        vec4 up;
//        vec4 forward;
//
//        vec3 eulerAngles;
//        vec3 localEulerAngles;
//
//        quat rotation;
//        quat localRotation;
//
//        vec4 lossyScale;
//        vec4 localScale;
//
//        mat4 m_Matrix;
//        mat4 localToWorldMatrix;
//        mat4 worldToLocalMatrix;
//    };
//}
//
//
//#endif//VULKAN_ENGINE_TRANSFROM_H
