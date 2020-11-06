//
// Created by HADMARINE on 2020-11-06.
//

#include <include/Engine/Scene/Components/BoxCollider.hpp>
#include "include/Engine/Scene/Manager/ColliderManager.hpp"
#include "include/Engine/Scene/Components/Base/Component.h"
#include "include/Engine/Scene/Components/RigidBody2D.hpp"
#include "include/Engine/SharkSystem.hpp"

namespace SharkEngine::Core {
    using namespace std;

    void ColliderManager::MountCollider(BoxCollider *colA,
                                        BoxCollider *colB) {
        CLogger::Info("MOUNT COLLIDER");
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

    namespace CalculateFunctions {
        Vec2 getHeightVector(BoxCollider *a) {
            Vec2 ret;
            ret.x = a->GetLeftTopPos().y * sinf(a->GetRotation()) / 2;
            ret.y = a->GetRightBottomPos().y * cosf(a->GetRotation()) / 2;
            return ret;
        }

        Vec2 getWidthVector(BoxCollider *a) {
            Vec2 ret;
            ret.x = a->GetRightBottomPos().x * cosf(a->GetRotation()) / 2;
            ret.y = a->GetLeftTopPos().x * sinf(a->GetRotation()) / 2;
            return ret;
        }

        Vec2 getUnitVector(Vec2 a) {
            Vec2 ret;
            float size;
            size = sqrt(pow(a.x, 2) + pow(a.y, 2));
            ret.x = a.x / size;
            ret.y = a.y / size;
            return ret;
        }

#define PI 3.14159265358979323846f

        float deg_to_rad(float deg) {
            return deg * PI / 180;
        }

        float rad_to_deg(float rad) {
            return rad * 180 / PI;
        }

        float dotProduct(Vec2 &l, Vec2 &r) {
            return l.x * r.x + l.y * r.y;
        }

        Vec2 crossProduct(Vec2 l, Vec2 r) {
            // TODO : TEST THIS
            Vec2 v;
            v.x = l.x * r.y;
            v.y = l.y * r.x;
            return v;
        }

        struct XYMinMax {
            struct xy {
                float x;
                float y;
            };
            xy min;
            xy max;
        };

        void setMinMax(BoxCollider *collider, XYMinMax &xymm) {
            xymm.max.x = collider->GetLeftTopPos().x;
            xymm.max.y = collider->GetLeftTopPos().y;
            xymm.min.x = collider->GetRightBottomPos().x;
            xymm.min.y = collider->GetRightBottomPos().y;
        }
    }

    void evalCollision(BoxCollider *_A, BoxCollider *_B) {
        using namespace CalculateFunctions;

        auto A = _A->GetOwner()->GetComponent<RigidBody2D>();
        auto B = _B->GetOwner()->GetComponent<RigidBody2D>();

        Vec2 rv = B->GetVelocity() - A->GetVelocity();
        auto normal = crossProduct((_A->GetLeftTopPos() + _A->GetRightBottomPos()) / 2,
                                   (_A->GetLeftTopPos() + _A->GetRightBottomPos()) / 2);

        // Calculate relative velocity in terms of the normal direction
        float velAlongNormal = dotProduct(rv, normal);

        // Do not resolve if velocities are separating
        if (velAlongNormal > 0)
            return;

        // Calculate restitution
        float e = min(A->GetRestitution(), B->GetRestitution());

        // Calculate impulse scalar
        float j = -(1 + e) * velAlongNormal;
        j /= 1 / A->GetMass() + 1 / B->GetMass();

        // Apply impulse
        Vec2 impulse = j * normal;
    }

    bool EvalAABB(BoxCollider *A, BoxCollider *B) {
        using namespace CalculateFunctions;

        XYMinMax a{}, b{};

        setMinMax(A, a);
        setMinMax(B, b);

        if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
        if (a.max.y < b.min.y || a.min.y > b.max.y) return false;

        return true;
    }

    bool EvalOBB(BoxCollider *A, BoxCollider *B) {
        using namespace CalculateFunctions;
        Vec2 dist = ((A->GetLeftTopPos() + A->GetRightBottomPos()) / 2) -
                    ((B->GetLeftTopPos() + B->GetRightBottomPos()) / 2);
        Vec2 vec[4] = {
                getHeightVector(A),
                getHeightVector(B),
                getWidthVector(A),
                getWidthVector(B)};
        Vec2 unit;
        for (auto &i : vec) {
            double sum = 0;
            unit = getUnitVector(i);
            for (auto &j : vec) {
                sum += abs(j.x * unit.x + j.y * unit.y);
            }
            if (abs(dist.x * unit.x + dist.y * unit.y) > sum) {
                return false;
            }
        }
        return true;
    }

    void ColliderManager::LateUpdate() {
        for (auto iter : this->colliderQueue) {
            BoxCollider *valA = *iter.begin();
            BoxCollider *valB = *++iter.begin();

            if (valA == nullptr || valB == nullptr) {
                CLogger::Debug("[ColliderMananger] Collider is NULL! - Skipping");
                continue;
            }

            bool isCollided;

            if (valA->GetRotation() == 0.0f && valB->GetRotation() == 0.0f) {
                isCollided = EvalAABB(valA, valB);
            } else {
                isCollided = EvalOBB(valA, valB);
            }

            if (isCollided) {
                if (!valA->GetIsTrigger() || !valB->GetIsTrigger()) {
                    evalCollision(valA, valB);
                    if (!valA->GetIsCollided()) {
                        valA->OnCollisionEnter();
                    } else {
                        valA->OnCollisionStay();
                    }

                    if (!valB->GetIsCollided()) {
                        valB->OnCollisionEnter();
                    } else {
                        valB->OnCollisionStay();
                    }
                } else {
                    if (!valA->GetIsCollided()) {
                        valA->OnTriggerEnter();
                    } else {
                        valA->OnTriggerStay();
                    }

                    if (!valB->GetIsCollided()) {
                        valB->OnTriggerEnter();
                    } else {
                        valB->OnTriggerStay();
                    }
                }
                valA->SetIsCollided(true);
                valB->SetIsCollided(true);
            } else {
                if (!valA->GetIsTrigger() || !valB->GetIsTrigger()) {
                    if (valA->GetIsCollided()) {
                        valA->OnCollisionExit();
                    }
                    if (valB->GetIsCollided()) {
                        valB->OnCollisionExit();
                    }
                } else {
                    if (valA->GetIsCollided()) {
                        valA->OnTriggerExit();
                    }

                    if (valB->GetIsCollided()) {
                        valB->OnTriggerExit();
                    }
                }

                valA->SetIsCollided(false);
                valB->SetIsCollided(false);
            }

        }
    }

//    void ColliderManager::TestObject() {
//        auto component = SHARK_SCENE_MGR->GetCurrentScene()->GetComponentArray<Component>();
//    }
}



