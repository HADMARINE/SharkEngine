//
// Created by joonl on 2020-11-06.
//

#include "BoxCollider.hpp"

void SharkEngine::Core::BoxCollider::Update() {
    // TODO : MOUNT ALL COLLIDERS
    auto colliderArray = GetOwner()->GetScene()->GetComponentArray<BoxCollider>();

    for(auto iter : *colliderArray) {
        if(iter == this) {
            continue;
        }
        GetOwner()->GetScene()->RegisterCollider(iter, this);
    }
}
