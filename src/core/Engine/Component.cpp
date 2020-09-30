//
// Created by EunwooSong on 2020-09-28.
//

#include "../../Component.h"
#include "../../Message.h"

using namespace SharkEngine::Core;

Component::Component(std::string _ID) {
    _id = _ID;
    Awake();
}
Component::~Component(void) {
}
const std::string Component::GetComponentID() const {
    return _id;
}
void Component::SetOwner(GameObject *obj) {
    m_pOwner = obj;
}
GameObject *Component::GetOwner() const {
    return m_pOwner;
}