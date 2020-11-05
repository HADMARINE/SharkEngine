//
// Created by EunwooSong on 2020-11-05.
//

#include "include/Engine/SystemManager/LineDebuggerManager.h"
#include "include/Engine/SharkSystem.hpp"

using namespace SharkEngine::Core;

SharkEngine::Core::LineDebuggerManager::LineDebuggerManager() {
    SHARK_DIRECT3D->GetLine()->SetWidth(5);
}
SharkEngine::Core::LineDebuggerManager::~LineDebuggerManager() {
}
void SharkEngine::Core::LineDebuggerManager::DrawBox(Vec2 p1, Vec2 p2, D3DCOLOR color) {
    Vec2 v1[2] = {{p1.x, p1.y}, {p1.x, p2.y}};
    Vec2 v2[2] = {{p1.x, p2.y}, {p2.x, p2.y}};
    Vec2 v3[2] = {{p2.x, p2.y}, {p2.x, p1.y}};
    Vec2 v4[2] = {{p2.x, p1.y}, {p1.x, p1.y}};

    LineStruct ver1;
    ver1.points[0] = v1[0];
    ver1.points[1] = v1[1];
    ver1.color = color;

    LineStruct ver2;
    ver2.points[0] = v2[0];
    ver2.points[1] = v2[1];
    ver2.color = color;

    LineStruct ver3;
    ver3.points[0] = v3[0];
    ver3.points[1] = v3[1];
    ver3.color = color;

    LineStruct ver4;
    ver4.points[0] = v4[0];
    ver4.points[1] = v4[1];
    ver4.color = color;

    drawLineList.push_back(ver1);
    drawLineList.push_back(ver2);
    drawLineList.push_back(ver3);
    drawLineList.push_back(ver4);
}
void SharkEngine::Core::LineDebuggerManager::Render() {
    for(auto iter : drawLineList)
        SHARK_DIRECT3D->GetLine()->Draw(iter.points, 2, iter.color);

    drawLineList.clear();
    drawLineList.resize(0);
}
