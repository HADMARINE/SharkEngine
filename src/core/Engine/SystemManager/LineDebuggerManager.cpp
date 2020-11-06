//
// Created by EunwooSong on 2020-11-05.
//

#include "include/Engine/SystemManager/LineDebuggerManager.h"
#include "include/Engine/SharkSystem.hpp"
#include "include/Engine/Scene/Components/Transform.h"

using namespace SharkEngine::Core;

SharkEngine::Core::LineDebuggerManager::LineDebuggerManager() {
    SHARK_DIRECT3D->GetLine()->SetWidth(5);
}
SharkEngine::Core::LineDebuggerManager::~LineDebuggerManager() {
}
void SharkEngine::Core::LineDebuggerManager::DrawBox(Vec2 p1, Vec2 p2, D3DCOLOR color) {
    D3DXVECTOR2 v1[2] = {{p1.x, p1.y}, {p1.x, p2.y}};
    D3DXVECTOR2 v2[2] = {{p1.x, p2.y}, {p2.x, p2.y}};
    D3DXVECTOR2 v3[2] = {{p2.x, p2.y}, {p2.x, p1.y}};
    D3DXVECTOR2 v4[2] = {{p2.x, p1.y}, {p1.x, p1.y}};

    LineStruct ver1;
    ver1.points[0] = v1[0];
    ver1.points[1] = v1[1];
    ver1.color = color;
    ver1.mat;D3DXMatrixTranslation(&ver1.mat, 0.0f, 0.0f, 0.0f);

    LineStruct ver2;
    ver2.points[0] = v2[0];
    ver2.points[1] = v2[1];
    ver2.color = color;
    ver2.mat = ver1.mat;

    LineStruct ver3;
    ver3.points[0] = v3[0];
    ver3.points[1] = v3[1];
    ver3.color = color;
    ver3.mat = ver1.mat;

    LineStruct ver4;
    ver4.points[0] = v4[0];
    ver4.points[1] = v4[1];
    ver4.color = color;
    ver4.mat = ver1.mat;

//    drawLineList.push_back(ver1);
//    drawLineList.push_back(ver2);
//    drawLineList.push_back(ver3);
//    drawLineList.push_back(ver4);
}
void SharkEngine::Core::LineDebuggerManager::Render() {
    //CheckMatrix
    D3DXMATRIX zeroMat;
    D3DXMatrixTranslation(&zeroMat, 0.0f, 0.0f, 0.0f);

    for(auto iter : drawLineList) {
        //if(iter.mat != zeroMat)
            //SHARK_DIRECT3D->GetLine()->Draw(iter.points, 2, &iter.mat, iter.color);
        //else
            SHARK_DIRECT3D->GetLine()->Draw(iter.points, 2, iter.color);

    }

    drawLineList.clear();
    drawLineList.resize(0);
}
void LineDebuggerManager::DrawBox(float p1, float p2, D3DCOLOR color, const D3DXMATRIX &mat) {
}
