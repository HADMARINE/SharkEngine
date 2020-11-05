//
// Created by EunwooSong on 2020-11-01.
//

#include "../../../../include/Engine/Scene/Components/Transform.h"
#include <d3dx9math.h>

using namespace SharkEngine::Core;

void Transform::Start() {
    Component::Start();
}
void Transform::Update() {
    Component::Update();
}
void Transform::Render() {
    Component::Render();

    if(!parent)
        UpdateTransformation();
}
void Transform::LateUpdate() {
    Component::LateUpdate();
}
void Transform::EndScene() {
    Component::EndScene();
}
void Transform::AddChild(Transform *iter) {
    iter->SetParent(this);
    children.push_back(iter);
}
void Transform::PopChild(Transform *iter) {
    children.erase(remove_if(children.begin(), children.end(), [&](auto i) {return i == iter;}), children.end());
}
void Transform::UpdateTransformation() {
    D3DXMatrixTransformation2D((D3DXMATRIX *) &mat, &scaleCenter, NULL, &scale, &rotationCenter, rotation, &localPos);

    worldPos = localPos;

    if (parent) {
        mat *= parent->mat;
        worldPos += parent->GetLocalPos();
    }

    if (children.size() > 0)
        for (auto& child : children)
            child->UpdateTransformation();
}
//glm::vec3 Transform::TransformDirection(glm::vec3) {
//    return glm::vec3();
//}
//glm::vec3 Transform::TransformDirection(glm::vec4) {
//    return glm::vec3();
//}
//glm::vec3 Transform::TransformDirection(float, float, float) {
//    return glm::vec3();
//}
//glm::vec3 Transform::InverseTransformDirection(glm::vec3) {
//    return glm::vec3();
//}
//glm::vec3 Transform::InverseTransformDirection(glm::vec4) {
//    return glm::vec3();
//}
//glm::vec3 Transform::InverseTransformDirection(float, float, float) {
//    return glm::vec3();
//}
//glm::vec3 Transform::TransformPoint(glm::vec3) {
//    return glm::vec3();
//}
//glm::vec3 Transform::TransformPoint(float, float, float) {
//    return glm::vec3();
//}
//glm::vec3 Transform::InverseTransformPoint(glm::vec3) {
//    return glm::vec3();
//}
//glm::vec3 Transform::InverseTransformPoint(float, float, float) {
//    return glm::vec3();
//}
//void Transform::LookAt(Transform * iter) {
//
//}
//void Transform::LookAt(glm::vec3) {
//
//}
//void Transform::Rotate(glm::vec3) {
//}
//void Transform::Rotate(float, float, float) {
//}
//void Transform::Translate(glm::vec3 position) {
//    m_Matrix = glm::translate(position);
//}
//void Transform::AddChild(Transform *iter) {
//    children.push_back(iter);
//}
//void Transform::SetParent(Transform* target, Transform* children) {
//}
