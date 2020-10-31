//
// Created by EunwooSong on 2020-11-01.
//

#include "../../../../include/Engine/Scene/Components/Transform.h"

using namespace SharkEngine::Core;

void Transform::Transform::Start() {
    Component::Start();
}
void Transform::Transform::Update() {
    Component::Update();
}
void Transform::Transform::Render() {
    Component::Render();
}
void Transform::Transform::LateUpdate() {
    Component::LateUpdate();
}
void Transform::Transform::EndScene() {
    Component::EndScene();
}
glm::vec3 Transform::Transform::TransformDirection(glm::vec3) {
    return glm::vec3();
}
glm::vec3 Transform::Transform::TransformDirection(glm::vec4) {
    return glm::vec3();
}
glm::vec3 Transform::Transform::TransformDirection(float, float, float) {
    return glm::vec3();
}
glm::vec3 Transform::Transform::InverseTransformDirection(glm::vec3) {
    return glm::vec3();
}
glm::vec3 Transform::Transform::InverseTransformDirection(glm::vec4) {
    return glm::vec3();
}
glm::vec3 Transform::Transform::InverseTransformDirection(float, float, float) {
    return glm::vec3();
}
glm::vec3 Transform::Transform::TransformPoint(glm::vec3) {
    return glm::vec3();
}
glm::vec3 Transform::Transform::TransformPoint(float, float, float) {
    return glm::vec3();
}
glm::vec3 Transform::Transform::InverseTransformPoint(glm::vec3) {
    return glm::vec3();
}
glm::vec3 Transform::Transform::InverseTransformPoint(float, float, float) {
    return glm::vec3();
}
void Transform::Transform::LookAt(Transform::Transform * iter) {
}
void Transform::Transform::LookAt(glm::vec3) {
}
void Transform::Transform::Rotate(glm::vec3) {
}
void Transform::Transform::Rotate(float, float, float) {
}
void Transform::Transform::RotateAround(glm::vec3, glm::vec3, float) {
}
void Transform::Transform::RotateAround(glm::vec3, float) {
}
void Transform::Transform::RotateAroundLocal(glm::vec3, float) {
}
void Transform::Transform::Translate(glm::vec3) {
}
