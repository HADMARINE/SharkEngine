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

    //LineStruct ver1 =

//    drawLineList.push_back(v1);
//    drawLineList.push_back(v2);
//    drawLineList.push_back(v3);
//    drawLineList.push_back(v4);
}
void SharkEngine::Core::LineDebuggerManager::Render() {
//    for(auto iter : drawLineList)
//        SHARK_DIRECT3D->GetLine()->Draw(iter, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
//
//    drawLineList.clear();
//    drawLineList.resize(0);
}
