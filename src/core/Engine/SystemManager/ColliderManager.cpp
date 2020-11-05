//
// Created by HADMARINE on 2020-11-06.
//

#include <include/Engine/Scene/Components/BoxCollider.hpp>
#include "include/Engine/SystemManager/ColliderManager.hpp"
#include "include/Engine/Scene/Components/Base/Component.h"

namespace SharkEngine::Core::Physics {
    using namespace std;

    void ColliderManager::MountCollider(BoxCollider *colA,
                                        BoxCollider *colB) {
        unordered_set<BoxCollider *> colliderSet;
        colliderSet.insert(colA);
        colliderSet.insert(colB);
        for (auto iter : this->colliderQueue) {
            if (iter == colliderSet) {
                CLogger::Debug("[ColliderManager] Collide case already exists!");
                return;
            }
        }
        this->colliderQueue.push_back(colliderSet);
    }

    vector<unordered_set<BoxCollider *>>
    ColliderManager::GetColliderQueue() {
        return this->colliderQueue;
    }

    void ColliderManager::eval() {
        for(auto iter : this->colliderQueue) {
            BoxCollider* valA = *iter.begin();
            BoxCollider* valB = *++iter.begin();

            bool isAABB = false;

            if(valA == nullptr || valB == nullptr) {
                CLogger::Debug("[ColliderMananger] Collider is NULL! - Skipping");
                continue;
            }

            Transform* ATrans = valA->GetOwner()->GetComponent<Transform>();
            Transform* BTrans = valB->GetOwner()->GetComponent<Transform>();

            if(ATrans->GetRotation() == 0.0f && BTrans->GetRotation() == 0.0f) {
                isAABB = true;
            }




        }
    }

}



