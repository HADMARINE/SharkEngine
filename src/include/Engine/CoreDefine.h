//
// Created by EunwooSong on 2020-10-02.
//

#ifndef VULKAN_ENGINE_COREDEFINE_H
#define VULKAN_ENGINE_COREDEFINE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <d3dx9.h>
#include <queue>
#include <array>
#include <iostream>
#include <random>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <functional>

//DELETE Macro
#define SAFE_DELETE(p)     \
    {                      \
        if (p) {           \
            delete (p);    \
            (p) = nullptr; \
        }                  \
    }

#define SAFE_DELETE_ARRAY(p) \
    {                        \
        if (p) {             \
            delete[](p);     \
            (p) = nullptr;   \
        }                    \
    }

#define SAFE_RELEASE(p)     \
    {                       \
        if (p) {            \
            (p)->Release(); \
            (p) = nullptr;  \
        }                   \
    }

using Vec2 = D3DXVECTOR2;

class Rect : public RECT {
public:
    Rect() {
        left = 0;
        right = 0;
        top = 0;
        bottom = 0;
    }

    Rect(int r, int b) {
        left = 0;
        right = r;
        top = 0;
        bottom = b;
    }

    Rect(int l, int t, int r, int b) {
        left = l;
        right = r;
        top = t;
        bottom = b;
    }

    Rect(Vec2 lt, Vec2 rb) {
        left = lt.x;
        top = lt.y;
        right = rb.x;
        bottom = rb.y;
    }

    bool Intersects(Rect r) {
        Rect intersect;
        return IntersectRect(&intersect, this, &r);
    }

    bool Contain(Vec2 p) {
        return left < p.x&& p.x < right&& top < p.y&& p.y < bottom;
    }

    Rect offset(Vec2 p) {
        Rect r = *this;
        r.left += p.x;
        r.right += p.x;
        r.top += p.y;
        r.bottom += p.y;

        return r;
    }

    float width() {
        return right - left;
    }

    float height() {
        return bottom - top;
    }

    Vec2 center() {
        return Vec2(width(), height()) / 2.0f;
    }
};

//Math Macro
template <class T>
inline T Clamp(T v1, T max) {
    return v1 > max ? max : v1;
}

template <class T>
inline T Clamp(T v1, T max, T min) {
    v1 = v1 > max ? max : (v1 < min) ? min : v1;
    return v1;
}

template <class T>
inline T Lerp(T p1, T p2, float v) {
    v = Clamp(v, 1.0f);
    return p1 + (p2 - p1) * v;
}

inline float Random(float form, float to) {
    static std::default_random_engine random(timeGetTime());
    std::uniform_real_distribution<float> d(form, to);
    return d(random);
}

inline float Angle(Vec2 p1, Vec2 p2) {
    Vec2 p = p2 - p1;
    return atan2(p.y, p.x);
}

#endif//VULKAN_ENGINE_COREDEFINE_H
