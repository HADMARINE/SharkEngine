//
// Created by EunwooSong on 2020-11-04.
//

#include "include/Graphic/DirectX9Core/WindowsApplication.h"
#include "GlobalPreferences.hpp"
#include "include/Engine/CoreDefine.h"
LRESULT WindowsApplication::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
    switch (iMessage)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
HWND WindowsApplication::FloatWindow(int cmdShow) {
    hInstance = GetModuleHandle(NULL);

    hWnd = CreateWindow(
            TEXT(GlobalPreferences::ENGINE_NAME),
            TEXT(GlobalPreferences::APPLICATION_NAME),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            GlobalPreferences::SCREEN_WIDTH,
            GlobalPreferences::SCREEN_HEIGHT,
            NULL,
            (HMENU)NULL,
            hInstance,
            NULL
    );

    ShowWindow(hWnd, cmdShow);
    return hWnd;
}
void WindowsApplication::Initialize() {
    ZeroMemory(&hInstance, sizeof(hInstance));
    hInstance = GetModuleHandle(NULL);

    WNDCLASS wndClass;

    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hInstance = hInstance;
    wndClass.lpfnWndProc = WndProc;
    wndClass.lpszClassName = GlobalPreferences::ENGINE_NAME;
    wndClass.lpszMenuName = NULL;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wndClass);

    msg = {0, };
}
WindowsApplication *WindowsApplication::Instance() {
    static WindowsApplication* iter = new WindowsApplication();
    return iter;
}
MSG WindowsApplication::CheckMessage() {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg;
}