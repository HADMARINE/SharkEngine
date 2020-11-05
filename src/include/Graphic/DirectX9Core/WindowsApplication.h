//
// Created by EunwooSong on 2020-11-04.
//

#ifndef SHARKENGINE_WINDOWSAPPLICATION_H
#define SHARKENGINE_WINDOWSAPPLICATION_H

#include <Windows.h>

#define WINDOWS_APPLICATION WindowsApplication::Instance()

class WindowsApplication {
public:
    WindowsApplication(){}
    ~WindowsApplication(){}

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
    HWND FloatWindow(int cmdShow);

    void Initialize();
    MSG CheckMessage();

    static WindowsApplication* Instance();

    HWND GetHWND()             {return hWnd;}
    HINSTANCE GetHINSTANCE()   {return hInstance;}

private:
    HWND hWnd;
    HINSTANCE hInstance;
    MSG msg;
};


#endif//SHARKENGINE_WINDOWSAPPLICATION_H
