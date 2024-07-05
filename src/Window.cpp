#include "Window.h"
#include "Core.h"
#include "WndProc.h"

const char windowClassName[] = "Simple Renderer";
const char windowName[] = "Renderer";

Window::Window(HINSTANCE hInstance) : 
    _width(cfgDefaultWindowWidth),
    _height(cfgDefaultWindowHeight),
    _newWidth(cfgDefaultWindowWidth),
    _newHeight(cfgDefaultWindowHeight){

        RegisterWindowClass(hInstance);
}

Window::Window(HINSTANCE hInstance, int width, int height) : 
    _width(width),
    _height(height),
    _newWidth(width),
    _newHeight(height){

        RegisterWindowClass(hInstance);
}

HWND Window::RegisterWindowClass(HINSTANCE hInstance){
    WNDCLASS windowClass = {};
    windowClass.lpfnWndProc = WndProc;
    windowClass.hInstance = hInstance;
    windowClass.lpszClassName = windowClassName;
    RegisterClass(&windowClass);
    RECT rect = {0, 0, _width, _height};
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, true);
    HWND window = CreateWindowEx(
            0,
            windowClassName,    
        windowName,
        WS_OVERLAPPEDWINDOW,   
        CW_USEDEFAULT, CW_USEDEFAULT,rect.right-rect.left, rect.bottom-rect.top,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    _handle = window;
    return window;
}