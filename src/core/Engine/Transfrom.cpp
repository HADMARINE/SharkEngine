////
//// Created by EunwooSong on 2020-10-03.
////
//
//#include "../../Transfrom.h"
//#include "../../../Message.h"
//
//using namespace SharkEngine::Core;
//
//void SharkEngine::Core::Components::Transform::Awake() {
//
//}
//void SharkEngine::Core::Components::Transform::Update() {
//}
//void SharkEngine::Core::Components::Transform::Render() {
//}
//void SharkEngine::Core::Components::Transform::LateUpdate() {
//}
//void SharkEngine::Core::Components::Transform::EndScene() {
//}
//void SharkEngine::Core::Components::Transform::OnMessage(SharkEngine::Core::Message * mes) {
//    //switch (mes->type) {
//    //    case SharkEngine::Core::PROCESS_VALUE_MESSAGE:
//    //        break;
//    //}
//}
//void SharkEngine::Core::Components::Transform::SendMessage() {
//}
//vec3 SharkEngine::Core::Components::Transform::InverseTransformDirection(vec3 direction) {
//    return TransformDirection(direction) * -1.0f;
//}
//vec3 SharkEngine::Core::Components::Transform::InverseTransformDirection(float x, float y, float z) {
//    return TransformDirection(x, y, z);
//}
//vec3 SharkEngine::Core::Components::Transform::InverseTransformDirection(vec4 direction) {
//    return TransformDirection(direction);
//}
//vec3 SharkEngine::Core::Components::Transform::InverseTransformPoint(vec3 position) {
//    return TransformPoint(position);
//}
//vec3 SharkEngine::Core::Components::Transform::InverseTransformPoint(float x, float y, float z) {
//    return TransformPoint(vec3(x, y, z));
//}
//void SharkEngine::Core::Components::Transform::LookAt(SharkEngine::Core::Components::Transform *) {
//}
//void SharkEngine::Core::Components::Transform::LookAt(vec3) {
//}
//void SharkEngine::Core::Components::Transform::Rotate(vec3) {
//}
//void SharkEngine::Core::Components::Transform::Rotate(float, float, float) {
//}
//void SharkEngine::Core::Components::Transform::RotateAround(vec3, vec3, float) {
//}
//void SharkEngine::Core::Components::Transform::RotateAround(vec3, float) {
//}
//void SharkEngine::Core::Components::Transform::RotateAroundLocal(vec3, float) {
//}
//void SharkEngine::Core::Components::Transform::Translate(vec3 position) {
//    this->position += vec4(position.x, position.y, position.z, 0);
//    this->position.w = 1;
//}
//vec3 SharkEngine::Core::Components::Transform::TransformDirection(vec4 direction) {
//    return localToWorldMatrix * direction;
//}
//vec3 SharkEngine::Core::Components::Transform::TransformDirection(float x, float y, float z) {
//    return TransformDirection(glm::vec4(x, y, z, 0));
//}
//vec3 SharkEngine::Core::Components::Transform::TransformDirection(vec3 direction) {
//    return TransformDirection(glm::vec4(direction.x, direction.y, direction.z, 0));
//}
//vec3 SharkEngine::Core::Components::Transform::TransformPoint(vec3 position) {
//    return localToWorldMatrix * vec4(position.x, position.y, position.z, 1.0f);
//}
//vec3 SharkEngine::Core::Components::Transform::TransformPoint(float x, float y, float z) {
//    return TransformPoint(vec3(x, y, z));
//}
