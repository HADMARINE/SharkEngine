//
// Created by EunwooSong on 2020-11-26.
//

#ifndef SHARKENGINE_SHARKMATH_H
#define SHARKENGINE_SHARKMATH_H

#include "stdafx.hpp"
#include <d3d9.h>
#include <random>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <Windows.h>

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

    Rect Offset(Vec2 p) {
        Rect r = *this;
        r.left += p.x;
        r.right += p.x;
        r.top += p.y;
        r.bottom += p.y;

        return r;
    }

    float Width() {
        return right - left;
    }

    float Height() {
        return bottom - top;
    }

    Vec2 Center() {
        return Vec2(Width(), Height()) / 2.0f;
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

#endif//SHARKENGINE_SHARKMATH_H
