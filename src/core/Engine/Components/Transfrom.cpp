//
// Created by EunwooSong on 2020-10-03.
//

#include "../../../Transfrom.h"

SharkEngine::Core::Components::Transform::~Transform() {

}
void SharkEngine::Core::Components::Transform::Awake() {

}
void SharkEngine::Core::Components::Transform::Update() {
}
void SharkEngine::Core::Components::Transform::Render() {
}
void SharkEngine::Core::Components::Transform::LateUpdate() {
}
void SharkEngine::Core::Components::Transform::EndScene() {
}
void SharkEngine::Core::Components::Transform::OnMessage(SharkEngine::Core::Message *) {
}
void SharkEngine::Core::Components::Transform::SendMessage() {
}
vec3 SharkEngine::Core::Components::Transform::InverseTransformDirection(vec3) {
    return glm::vec3();
}
vec3 SharkEngine::Core::Components::Transform::InverseTransformDirection(float, float, float) {
    return glm::vec3();
}
vec3 SharkEngine::Core::Components::Transform::InverseTrnasformPoint(vec3) {
    return glm::vec3();
}
vec3 SharkEngine::Core::Components::Transform::InverseTrnasformPoint(float, float, float) {
    return glm::vec3();
}
void SharkEngine::Core::Components::Transform::LookAt(SharkEngine::Core::Components::Transform *) {
}
void SharkEngine::Core::Components::Transform::LookAt(vec3) {
}
void SharkEngine::Core::Components::Transform::Rotate(vec3) {
}
void SharkEngine::Core::Components::Transform::Rotate(float, float, float) {
}
void SharkEngine::Core::Components::Transform::RotateAround(vec3, vec3, float) {
}
void SharkEngine::Core::Components::Transform::RotateAround(vec3, float) {
}
void SharkEngine::Core::Components::Transform::RotateAroundLocal(vec3, float) {
}
void SharkEngine::Core::Components::Transform::Translate(vec3) {
}
vec3 SharkEngine::Core::Components::Transform::TransformDirection(vec3) {
    return glm::vec3();
}
vec3 SharkEngine::Core::Components::Transform::TransformDirection() {
    return glm::vec3();
}
