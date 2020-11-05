//
// Created by EunwooSong on 2020-11-05.
//

#ifndef SHARKENGINE_LINEDEBUGGERMANAGER_H
#define SHARKENGINE_LINEDEBUGGERMANAGER_H

#include "../CoreDefine.h"
#include <d3d9.h>
#include <d3dx9.h>

namespace SharkEngine::Core {
    struct LineStruct {
        Vec2 points[2];
        D3DCOLOR color;
    };

    class LineDebuggerManager {
    public:
        LineDebuggerManager();
        ~LineDebuggerManager();

        void DrawBox(Vec2 p1, Vec2 p2, D3DCOLOR color);
        void Render();

    private:
        std::vector<LineStruct> drawLineList;
    };
}// namespace SharkEngine::Core


#endif//SHARKENGINE_LINEDEBUGGERMANAGER_H
