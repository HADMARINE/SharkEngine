//
// Created by EunwooSong on 2020-11-01.
//

#include "../../../../include/Engine/Scene/Components/Transform.h"

using namespace SharkEngine::Core;

void Transform::Start() {
    Component::Start();
}
void Transform::Update() {
    Component::Update();
}
void Transform::Render() {
    Component::Render();
}
void Transform::LateUpdate() {
    Component::LateUpdate();
}
void Transform::EndScene() {
    Component::EndScene();
}
glm::vec3 Transform::TransformDirection(glm::vec3) {
    return glm::vec3();
}
glm::vec3 Transform::TransformDirection(glm::vec4) {
    return glm::vec3();
}
glm::vec3 Transform::TransformDirection(float, float, float) {
    return glm::vec3();
}
glm::vec3 Transform::InverseTransformDirection(glm::vec3) {
    return glm::vec3();
}
glm::vec3 Transform::InverseTransformDirection(glm::vec4) {
    return glm::vec3();
}
glm::vec3 Transform::InverseTransformDirection(float, float, float) {
    return glm::vec3();
}
glm::vec3 Transform::TransformPoint(glm::vec3) {
    return glm::vec3();
}
glm::vec3 Transform::TransformPoint(float, float, float) {
    return glm::vec3();
}
glm::vec3 Transform::InverseTransformPoint(glm::vec3) {
    return glm::vec3();
}
glm::vec3 Transform::InverseTransformPoint(float, float, float) {
    return glm::vec3();
}
void Transform::LookAt(Transform * iter) {

}
void Transform::LookAt(glm::vec3) {

}
void Transform::Rotate(glm::vec3) {
}
void Transform::Rotate(float, float, float) {
}
void Transform::RotateAround(glm::vec3, glm::vec3, float) {
}
void Transform::RotateAround(glm::vec3, float) {
}
void Transform::RotateAroundLocal(glm::vec3, float) {
}
void Transform::Translate(glm::vec3) {
}
