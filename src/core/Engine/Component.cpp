//
// Created by EunwooSong on 2020-09-28.
//

#include "../../Component.h"
using name
Component::Component(std::string _ID) {
}
Component::~Component(void) {
}
const std::string Component::GetComponentID() const {
    return std::string();
}
void Component::SetOwner(GameObject *obj) {
}
GameObject *Component::GetOwner() const {
    return nullptr;
}
void Component::Awake(void) {
}
void Component::Update(void) {
}
void Component::Render(void) {
}
void Component::OnMessage() {
}
void Component::SendMessage() {
}
